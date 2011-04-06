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
#include "conf.h"

char * current_time();

int main(int argc, char * argv[]) {	
	opts_t opts;
	if(getopts(&opts, argc, argv) == 0) {
		#ifdef __DEBUG__
			printf("Command line arguments parsed successfully\n");
		#endif
	} else {
		//printf("Some errors in parsing command line arguments\n");
		return -1;
	}
	prefs_t prefs;
	prefs.buf = (char *) malloc(20000 * sizeof(char));
	prefs.buf_size = PREFS_BUFFER_SIZE;

	prefs.first_cs_size = AT_EXE_FIRST_CS_SIZE;
	prefs.hash = AT_EXE_HASH;
	prefs.skip_bytes = AT_EXE_SKIP_BYTES;
	prefs.sign_size = AT_EXE_SIGNATURE_LENGTH;
	int sign[PREFS_SIGN_BUF_SIZE] = AT_EXE_SIGNATURE;
	prefs.sign = &sign[0];
	
	printf("%s: Starting files processing\n", current_time());
	
	if(opts.mode == HASH) {
		walk_path(opts.path, hashcode_checker, &prefs);
	} else if(opts.mode == SIGNATURE) {
		walk_path(opts.path, signature_checker, &prefs);
	}	

	printf("%s: All files processed\n", current_time());

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
	sprintf_s(buf, "%2d:%2d:%2d %4d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return buf;
}

