/*
 * clipstore - A lightweight CLI snippet manager
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
