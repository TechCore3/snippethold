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
#include <termios.h>

#define MAX_CONTENT_LEN 65536

void print_usage() {
    puts("snippethold usage:");
    puts("  snippethold -s  <file> <contents>");
    puts("	snippethold -ss <file> <contents> <stdin:password>");
    puts("  snippethold -S  <file>");
    puts("	snippethold -Ss <file> <stdin:password>");
    puts("  snippethold -l");
    puts("  snippethold -r  <file>");
}

int main(int argc, char *argv[]){
	const char *home = getenv("HOME");
	char file[1024];
	char dir_path[512];
	char password[128]; // too short?
	char *content = malloc(MAX_CONTENT_LEN); 
	
	if (home == NULL || '\0' == home[0]){
		home = "/tmp";
	}

	if (argc < 2) {
        print_usage();
        return 1;
    }	

	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
	        print_usage();
	        return 0;
	    }

    snprintf(dir_path, sizeof(dir_path), "%s/.snippethold", home);

    if (access(dir_path, F_OK)){
		mkdir(dir_path, 0700);
	}

	if (!strcmp(argv[1], "-l")) {
	        return get_all_entries(dir_path);
	    }
	    
	if (argc < 3) {
	        print_usage();
	        return 1;
	    }
	    
	    snprintf(file , sizeof(file), "%s/%s",dir_path, argv[2]);
	    

	 if (!strcmp(argv[1], "-S")){
		return get_single_entry(file);
	}
	 if (!strcmp(argv[1], "-Ss")){
	 	fputs("enter password(to decrypt with): ", stdout);
	 	fflush(stdout);
	 	
	 	struct termios term;
	 	tcgetattr(fileno(stdin), &term);

	 	term.c_lflag &= ~ECHO;
	 	tcsetattr(STDIN_FILENO, TCSANOW, &term);// gotta love POSIX	

	 	fgets(password, sizeof(password), stdin);
	 	putchar('\n');
	 	term.c_lflag |= ECHO;
	 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
		password[strcspn(password, "\n")] = '\0';
		
		int result =  get_single_entry_crypt(file, password);
		free(content);
		return result;
	 }
	 if (!strcmp(argv[1], "-r")){
		return remove_entry(file);
	} 
	if (argc < 4) {
		print_usage();
		return 1;
	}
	if (!strcmp(argv[1], "-s")){
		snprintf(content, MAX_CONTENT_LEN,"%s", argv[3]); 
		return write_file(content, file);
	}
	if (!strcmp(argv[1], "-ss")){
		snprintf(content, MAX_CONTENT_LEN, "%s", argv[3]);
		
		fputs("enter password(to encrypt with): ", stdout);
		fflush(stdout);
		
		struct termios term;
		tcgetattr(fileno(stdin), &term);

		term.c_lflag &= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);

		fgets(password, sizeof(password), stdin);
		putchar('\n');
		
		term.c_lflag |= ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		password[strcspn(password, "\n")] = '\0';
		
		int result =  write_file_crypt(content, file, password);
		free(content);
		return result;
	}
		print_usage();
		return 0;
}
