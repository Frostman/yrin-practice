#include <windows.h>
#include <string>

#include <tlhelp32.h>

#include "file_util.h"

int is_process_runned(std::string process_name){
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int countProcess = 0;
	if(Process32First(hSnapshot, &pe)){
		while(Process32Next(hSnapshot, &pe)){ 		
			if(!strcmp(pe.szExeFile, process_name.c_str())){ 
				return true;
			}
		}
	}
	return false;
}

void run_process(std::string process_full_path, std::string dir, std::string process_name){
	std::string path = dir + "/" + process_name;

	if(0 != file_copy(process_full_path, path)){
		printf("Can't copy file from '%s' to '%s'\n", process_full_path.c_str(), path.c_str());
		return;
	}

	STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;

    memset(&startup_info, 0, sizeof(startup_info));
    memset(&process_info, 0, sizeof(process_info));
    startup_info.cb = sizeof(startup_info);	
		
	if(!CreateProcess(path.c_str(), (LPSTR) "", 0, 0, FALSE, CREATE_DEFAULT_ERROR_MODE, 0, 0, &startup_info, &process_info)){
		printf("Error in '%s': %d", path.c_str(), GetLastError());
	}

	printf("New process created successfully: %s\n", process_name.c_str());
}
