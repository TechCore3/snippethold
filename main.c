#include "clipstore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void print_usage(void) {
    puts("clipstore usage:");
    puts("  clipstore -s <file> <contents>");
    puts("  clipstore -S <file>");
    puts("  clipstore -l");
    puts("  clipstore -r <file>");
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

    snprintf(dir_path, sizeof(dir_path), "%s/.clipstore", home);

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
