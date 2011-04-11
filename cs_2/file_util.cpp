#include <windows.h>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "md5.h"

std::string get_file_name(std::string path) {
	int i = 0;
	for(i = path.length(); i >= 0; i--){
		if (path[i] == '\\'){			
			break;
		}
	}
	return path.substr(i + 1);
}

std::string get_file_dir(std::string path) {
	int i = 0;
	for(i = path.length(); i >= 0; i--){
		if (path[i] == '\\'){			
			break;
		}
	}
	return path.substr(0, i);
}

std::string get_file_attrs(std::string path) {
	int file_attrs = 1;
	std::stringstream attrs;

	WIN32_FIND_DATA file_data;
	HANDLE file_handler;		
	file_handler = FindFirstFile(path.c_str(), &file_data);
	if (file_handler != INVALID_HANDLE_VALUE){		
		attrs << file_data.dwFileAttributes;		
		FindClose(file_handler);
	} 

	return attrs.str();
}

std::string get_file_hash(std::string path) {
	HANDLE file_mapping, file_handler;            
	LPVOID file_base;                      
	PIMAGE_DOS_HEADER dos_header;        
	PIMAGE_NT_HEADERS nt_header;         
	IMAGE_OPTIONAL_HEADER op_header;     
	PIMAGE_SECTION_HEADER p_sec_header;  	

	file_handler = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL,NULL);
	if(file_handler == INVALID_HANDLE_VALUE){
		printf("ERROR: Could not open specified file\n");
	}

	file_mapping = CreateFileMapping(file_handler, NULL, PAGE_READONLY, 0, 0, NULL);
	file_base = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);
	dos_header = (PIMAGE_DOS_HEADER)file_base;

	if(dos_header->e_magic != IMAGE_DOS_SIGNATURE){
		printf("ERROR: Specified file is not a valid PE-EXE file\n");
	}

	nt_header = (PIMAGE_NT_HEADERS)((DWORD)(dos_header) + (dos_header->e_lfanew));
	op_header = nt_header->OptionalHeader;	

	MD5 md5 = MD5();

	int i = 0;
	for(p_sec_header = IMAGE_FIRST_SECTION(nt_header), i = 0; i < nt_header->FileHeader.NumberOfSections; i++, p_sec_header++){   				
		if(p_sec_header->SizeOfRawData!=0) {			
			char *p = (char *)((DWORD)dos_header + p_sec_header->PointerToRawData);			
			int size = p_sec_header->SizeOfRawData;
			char * buf = (char *) malloc(size + 1);
			for(int i = 0; i < size; i++, p++) {
				buf[i] = (*p);
			}

			md5.update(buf, size);
		}
	}

	UnmapViewOfFile(file_base);
	CloseHandle(file_mapping);
	CloseHandle(file_handler);		

	return md5.finalize().hexdigest();
}