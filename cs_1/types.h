#ifndef TYPES_H
#define TYPES_H

#include <windows.h>
#include <string>

enum scan_mode {
	NONE,
	HASH,
	SIGNATURE
};

typedef struct opts_s {
	scan_mode mode;
	std::string path;
} opts_t;

typedef struct code_section_s {
	int idx;
	char * start;
	int size;
} code_section_t;

typedef struct pe_file_s {
	bool valid;
	HANDLE file, file_mapping;
	LPVOID mem_base;                    //Pointer to memory of mapped file
	PIMAGE_DOS_HEADER dos_header;        
	PIMAGE_NT_HEADERS nt_header;         
	PIMAGE_SECTION_HEADER sec_table_header;   //Section Header or Section Table Header
	
} pe_file_t;


#endif /* TYPES_H */