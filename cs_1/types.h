#ifndef TYPES_H
#define TYPES_H

#include <windows.h>
#include <string>

#include "conf.h"

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
	bool cs_allocated;
	code_section_t ** cs;
	int non_empty_cs;
} pe_file_t;

typedef struct prefs_s {
	char * buf;
	int buf_size;

	int first_cs_size;

	std::string hash;

	int skip_bytes;
	int sign_size;
	int * sign;
} prefs_t;


#endif /* TYPES_H */