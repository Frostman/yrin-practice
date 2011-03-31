#include <windows.h>
#include <string>

#include "types.h"

void discover_pe_file(std::string path, pe_file_t * pe, int code_sections) {			
	pe->valid = false;
	pe->file = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(pe->file == INVALID_HANDLE_VALUE){
		#ifdef __DEBUG__
			printf("Error: Could not open specified file (%s)\n", path.c_str());
		#endif
		return;
	}
		
	pe->file_mapping = CreateFileMapping(pe->file, NULL, PAGE_READONLY, 0, 0, NULL);
	pe->mem_base = MapViewOfFile(pe->file_mapping, FILE_MAP_READ, 0, 0, 0);
	
	pe->dos_header = (PIMAGE_DOS_HEADER) pe->mem_base; // 0x04000000	
	if(pe->dos_header == 0 || pe->dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
		#ifdef __DEBUG__
			printf("Error: Sprecified file is not a valid PE-EXE file (%s)\n", path.c_str());
		#endif
		return;
	}

	//Offset of NT Header at 0x3c location in DOS header specified by e_lfanew
	//PE Header = pe->dos_header + RVA address of PE header	
	pe->nt_header = (PIMAGE_NT_HEADERS)((DWORD)(pe->dos_header) + (pe->dos_header->e_lfanew));
	if(pe->nt_header->Signature == IMAGE_NT_SIGNATURE) {		
		int i = 0;
		pe->sec_table_header = IMAGE_FIRST_SECTION(pe->nt_header);
		for(;i < min(code_sections, pe->nt_header->FileHeader.NumberOfSections); i++, pe->sec_table_header++) {   
				if(pe->sec_table_header->SizeOfRawData != 0) {					
					char * start = (char *)((DWORD)pe->dos_header + pe->sec_table_header->PointerToRawData);
					int size = pe->sec_table_header->SizeOfRawData;
					
				}
		}
	}
	
	pe->valid = true;
}

void close_pe_file(pe_file_t * pe) {	
	UnmapViewOfFile(pe->mem_base);
	if(pe->file != INVALID_HANDLE_VALUE){
		CloseHandle(pe->file_mapping);
	}
	CloseHandle(pe->file);

//	free(pe);
}

void hashcode_checker(std::string path, void * prefs) {
	pe_file_t pe;
	discover_pe_file(path, &pe, 1);
	if(pe.valid) {
		printf("%s\n", path.c_str());
	}
	close_pe_file(&pe);
}

void signature_checker(std::string path, void * prefs) {
}