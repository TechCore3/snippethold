#include "clipstore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

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
		printf("%s\n", buffer);
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
