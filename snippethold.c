/*
 * snippethold - A lightweight CLI snippet manager
 * Copyright (C) 2026 TechCore3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "snippethold.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sodium.h>

int write_file(const char *content, const char *file){
	FILE *  target = fopen(file, "w");
	if (NULL == target){
		perror("error opening file");
		return 1;
	}
	if (fprintf(target, "%s", content) < 0){
		perror("error writing to file");
		fclose(target);
		return 1;
	}
	fclose(target);
	return 0;
}
int get_single_entry(const char *file){
	FILE * target = fopen(file, "r");
	if (NULL == target){
		perror("error opening/reading file");
		return 1;
	}
	char buffer[4096];
	while (NULL != fgets(buffer, sizeof(buffer), target)){
		printf("%s", buffer);
	}
	if (ferror(target)){
		perror("error reading file contents");
		fclose(target);
		return 1;
	}
	fclose(target);
	return 0;
}
int remove_entry(const char *file){
	if (remove(file)){
		perror("error removing file");
		return 1;
	}
	return 0;
}
int get_all_entries(const char *dir_path){
	DIR *dir = opendir(dir_path);
	if (NULL == dir) {
		perror("failed to open dir");
		return 1;
	}
	puts("saved snippets:");

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){
		if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")){
			continue;
		}
		printf(" %s\n", entry->d_name);
	}
	closedir(dir);
	return 0;
}





//ENCRYPTION FUNCITONS HERE
int write_file_crypt(const char *content, const char *file, const char *password){
	if (sodium_init() < 0) return 1;
	
	unsigned char salt[crypto_pwhash_SALTBYTES];
	randombytes_buf(salt, sizeof(salt));

	unsigned char key[crypto_secretbox_KEYBYTES];
	if (crypto_pwhash(key, sizeof(key),
                      password, strlen(password),
                      salt,
                      crypto_pwhash_OPSLIMIT_SENSITIVE,
                      crypto_pwhash_MEMLIMIT_SENSITIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
                      	return 1;
                      }
    size_t text_len = strlen(content);
    size_t cipher_len = crypto_secretbox_MACBYTES + text_len;

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof(nonce));

    unsigned char *ciphertext = malloc(cipher_len);
    if (!ciphertext){
    	sodium_memzero(key, sizeof(key));
    	return 1;
    }

    crypto_secretbox_easy(ciphertext, (const unsigned char*)content, text_len, nonce, key);
    
        sodium_memzero(key, sizeof(key));
    
        FILE * target = fopen(file, "wb");
        if (!target) {
            free(ciphertext);
            return 1;
        }
    
        fwrite(salt, 1, sizeof(salt), target);
        fwrite(nonce, 1, sizeof(nonce), target);
        fwrite(ciphertext, 1, cipher_len, target);
        fclose(target);
    
        free(ciphertext);
        return 0;
}

	int get_single_entry_crypt(const char *file, const char *password){
	
		if (sodium_init() < 0) return 1;
		FILE * target = fopen(file, "rb");
		if (!target){
			perror("failed to open file!");
			return 1;
		}
		unsigned char salt[crypto_pwhash_SALTBYTES];
		unsigned char nonce[crypto_secretbox_NONCEBYTES];

		if (fread(salt, 1, sizeof(salt), target) != sizeof(salt) || 
		fread(nonce, 1, sizeof(nonce), target) != sizeof(nonce)) {
			fprintf(stderr, "file header incomplete or invalid!\n");
			fclose(target);
			return 1;
		}
		fseek(target, 0, SEEK_END);

		long file_size = ftell(target);
		long header_size = sizeof(salt) + sizeof(nonce);

		if (file_size < header_size + crypto_secretbox_MACBYTES) {
			fprintf(stderr, "file is too short or corrupted!\n");
			fclose(target);
			return 1;
		}
		size_t cipher_len = file_size - header_size;
		unsigned char *ciphertext = malloc(cipher_len);
		if (!ciphertext){
			fclose(target);
			return 1;
		}
		fseek(target, header_size, SEEK_SET);
		if (fread(ciphertext, 1, cipher_len, target) != cipher_len){
			fprintf(stderr, "failed to read ciphertext!");
			free(ciphertext);
			fclose(target);
			return 1;
		}
		fclose(target);
		unsigned char key[crypto_secretbox_KEYBYTES];
		if (crypto_pwhash(key, sizeof(key), password, strlen(password), salt,
		crypto_pwhash_OPSLIMIT_SENSITIVE,
		crypto_pwhash_MEMLIMIT_SENSITIVE,
		crypto_pwhash_ALG_DEFAULT) != 0){
			fprintf(stderr, "out of memory during key derivation!\n");
			free(ciphertext);
			return 1;
		}
		size_t cleartext_len = cipher_len - crypto_secretbox_MACBYTES;
		unsigned char *cleartext = malloc(cleartext_len);
		if (!cleartext){
			sodium_memzero(key, sizeof(key));
			free(ciphertext);
			return 1;
		}
		if (crypto_secretbox_open_easy(cleartext, ciphertext, cipher_len, nonce, key) != 0) {
		        fprintf(stderr, "incorrect password or corrupted file!\n");
		        sodium_memzero(key, sizeof(key));
		        free(ciphertext);
		        free(cleartext);
		        return 1;
		    }
		    
		sodium_memzero(key, sizeof(key));
		free(ciphertext);
		fwrite(cleartext, 1, cleartext_len, stdout);
		fflush(stdout);
		sodium_memzero(cleartext, cleartext_len);
		free(cleartext);
		return 0;
	}
