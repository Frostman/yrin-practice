#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <windows.h>

#define __DEBUG__OFF

#define PROC_NAME "cs_3.exe"
#define PROC_COPY_NAME "cs_3_copy.exe"
#define WATCH_INTERVAL 15000

#include "file_util.h"
#include "process_util.h"

void watch_dog(std::string, std::string, std::string);

int main(int argc, char * argv[]) {	
	char buffer[1024];
	GetModuleFileName(NULL, buffer, sizeof(buffer));
	char * name;
	if (0 == strcmp(PROC_NAME, get_name(buffer).c_str())) {		
		name = PROC_COPY_NAME;
	} else {
		name = PROC_NAME;
	}
	
	watch_dog(buffer, get_path(buffer) , name);	
}

void watch_dog(std::string full_path, std::string dir, std::string name){
	for(;;) {
		if(!is_process_runned(name)) { 
			run_process(full_path, dir, name);
		}
		
		Sleep(WATCH_INTERVAL);
	}
}