#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <windows.h>

#define __DEBUG__OFF

#include "types.h"
#include "getopts.h"
#include "file_walker.h"
#include "md5.h"
#include "analyzer.h"

char * current_time();

int main(int argc, char * argv[]) {	
	opts_t opts;
	if(getopts(&opts, argc, argv) == 0) {
		#ifdef __DEBUG__
			printf("Command line arguments parsed successfully\n");
		#endif
	} else {
		printf("Some errors in parsing command line arguments\n");
	}	
	
	printf("%s: Starting files processing\n", current_time());

	walk_path(opts.path, hashcode_checker, NULL);

	printf("%s: All files processed\n", current_time());

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

char * current_time() {
	char buf[1024];
	SYSTEMTIME st;
    GetSystemTime(&st);
	sprintf(buf, "%2d:%2d:%2d %4d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return buf;
}

