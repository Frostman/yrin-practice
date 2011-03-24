#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>

#define __DEBUG__

#include "types.h"
#include "getopts.h"
#include "file_walker.h"
#include "md5.h"

void file_counter();

int hash_checker(std::string path);

int main(int argc, char * argv[]) {
	opts_t opts;
	if(getopts(&opts, argc, argv) == 0) {
		#ifdef __DEBUG__
			printf("Command line arguments parsed successfully\n");
		#endif
	} else {
		printf("Some errors in parsing command line arguments\n");
	}	

	walk_path(opts.path, file_handler);

	printf("All files processed\n");

// string	md5(string)

	_getch();
}

void file_counter() {
	static __declspec(thread) int count = 0;		
	count++;
	if(count == 1000) {
		printf("1000 file processed\n");
		count = 0;
	}
}

int hash_checker(std::string path) {

}

