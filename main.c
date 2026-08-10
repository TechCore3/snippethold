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


#include "snippethold.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void print_usage(void) {
    puts("snippethold usage:");
    puts("  snippethold -s <file> <contents>");
    puts("  snippethold -S <file>");
    puts("  snippethold -l");
    puts("  snippethold -r <file>");
}

int main(int argc, char *argv[]){
	const char *home = getenv("HOME");
	char file[1024];
	char dir_path[512];
	char content[4096];
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

    snprintf(dir_path, sizeof(dir_path), "%s/.config/snippethold", home);

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
	 if (!strcmp(argv[1], "-r")){
		return remove_entry(file);
	} 
	if (!strcmp(argv[1], "-s")){
		if (argc < 4) {
			print_usage();
			return 1;
		}
		snprintf(content, sizeof(content),"%s", argv[3]); 
		return write_file(content, file);
	}
		print_usage();
		return 0;
}
