#include <windows.h>
#include <string>
#include <sstream>

#include "types.h"

void write_int(std::stringstream *, int);

void append_sign_data(std::string path, sign_data_t data) {
	HANDLE hFile = CreateFile(path.c_str(), FILE_APPEND_DATA, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if ( hFile == INVALID_HANDLE_VALUE ){
		append_sign_data(path, data);
	} 

	std::stringstream buf;			

	write_int(&buf, data.dir.length());
	buf << data.dir.c_str();

	write_int(&buf, data.name.length());
	buf << data.name.c_str();

	write_int(&buf, data.hash_code.length());
	buf << data.hash_code.c_str();

	write_int(&buf, data.file_attrs.length());
	buf << data.file_attrs.c_str();

	std::string str = buf.str();

	printf("APPEND: %s\n", str.c_str());

	DWORD dwBytesWritten;
	if (! ::WriteFile(hFile, str.c_str(), str.length(), &dwBytesWritten, NULL) ){
		printf("ERROR: Appending sign to file '%s' failed with error: %d\n", path, GetLastError());
		return;
	}
	
	CloseHandle(hFile);
}

std::string read_str(char * p);

sign_data_t read_sign_data(std::string path) {	
	HANDLE file_mapping, file_handler;            
	LPVOID file_base;                      
	PIMAGE_DOS_HEADER dos_header;        
	PIMAGE_NT_HEADERS nt_header;         
	IMAGE_OPTIONAL_HEADER op_header;     
	PIMAGE_SECTION_HEADER p_sec_header;  

	sign_data_t data;

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

	int i = 0;
	for(p_sec_header = IMAGE_FIRST_SECTION(nt_header), i = 0; i < nt_header->FileHeader.NumberOfSections; i++, p_sec_header++){   				
		if(p_sec_header->SizeOfRawData!=0 && (i == nt_header->FileHeader.NumberOfSections - 1)) {			
			char *p = (char *)((DWORD)dos_header + p_sec_header->PointerToRawData);			
			int size = p_sec_header->SizeOfRawData;
			p += size; // overlay
									
			data.dir = read_str(p);
			p += data.dir.length() + 4;

			data.name = read_str(p);
			p += data.name.length() + 4;
		}
	}

	UnmapViewOfFile(file_base);
	CloseHandle(file_mapping);
	CloseHandle(file_handler);		

	return data;
}

std::string read_str(char * p) {
	std::string buf = "";
	for(int i = 0; i < 4; i++, p++) {
		buf += *p;
	}
	int len = atoi(buf.c_str());

	buf = "";
	for(int i = 0; i < len; i++, p++) {
		buf += *p;
	}

	return buf;
}


void write_int(std::stringstream * s, int t) {
	int i = 1000;
	while(i != 1) {
		if(t / i == 0) {
			(*s) << 0;
		} else {
			break;
		}
		i /= 10;
	}
	(*s) << t;
}