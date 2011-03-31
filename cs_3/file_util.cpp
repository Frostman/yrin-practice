#include <windows.h>
#include <string>
#include <iostream>

int file_copy(std::string from, std::string to){    
    HANDLE read_handle = CreateFile(from.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, 0);
    
	if(read_handle == INVALID_HANDLE_VALUE){        
        return -1;
    }

    HANDLE write_handle = CreateFile(to.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    
	if(write_handle == INVALID_HANDLE_VALUE){        
        CloseHandle(read_handle);
        return -2;
    }

    BOOL is_read = FALSE, is_write = FALSE;
    DWORD readed = 0, writed = 0;
    BYTE buff[4096];

    do{
        is_read = ReadFile(read_handle, buff, sizeof(buff), &readed, 0);
        if(is_read && readed){
            is_write = WriteFile(write_handle, buff, readed, &writed, 0);
        }
    } while(is_read && readed && is_write && writed);

    CloseHandle(read_handle);
    CloseHandle(write_handle);

    return 0;
}

std::string get_path(std::string path){
	size_t idx = path.find_last_of('\\');	

	if(idx > path.length()) {
		return path;
	}
	return path.substr(0, idx);				
}

std::string get_name(std::string path){
	size_t idx = path.find_last_of('\\');	

	if(idx > path.length()) {
		return path;
	}
	return path.substr(idx + 1);				
}
