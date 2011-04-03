#include <windows.h>
#include <string>

#include "types.h"
#include "md5.h"

void discover_pe_file(std::string path, pe_file_t * pe, int code_sections) {			
	pe->valid = false;	
	pe->cs_allocated = false;
	pe->non_empty_cs = 0;	

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
	
	pe->nt_header = (PIMAGE_NT_HEADERS)((DWORD)(pe->dos_header) + (pe->dos_header->e_lfanew));
	if(pe->nt_header->Signature == IMAGE_NT_SIGNATURE) {		
		int i = 0;
		int limit = min(code_sections, pe->nt_header->FileHeader.NumberOfSections);
		pe->cs = (code_section_t **) malloc(limit * sizeof(code_section_t));
		pe->cs_allocated = true;
		pe->sec_table_header = IMAGE_FIRST_SECTION(pe->nt_header);
		for(;i < limit; i++, pe->sec_table_header++) {   
				if(pe->sec_table_header->SizeOfRawData != 0) {					
					char * start = (char *)((DWORD)pe->dos_header + pe->sec_table_header->PointerToRawData);
					int size = pe->sec_table_header->SizeOfRawData;
					
					pe->cs[pe->non_empty_cs] = (code_section_t *) malloc(sizeof(code_section_t));
					pe->cs[pe->non_empty_cs]->idx = pe->non_empty_cs;
					pe->cs[pe->non_empty_cs]->start = start;
					pe->cs[pe->non_empty_cs]->size = size;

					pe->non_empty_cs++;
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

	if(pe->cs_allocated) {
		for(int i = 0; i < pe->non_empty_cs; i++) {
			free(pe->cs[i]);
		}
		free(pe->cs);
	}
}

void hashcode_checker(std::string path, prefs_t * prefs) {
	pe_file_t pe;	
	discover_pe_file(path, &pe, 1);
	if(pe.valid && pe.cs_allocated && pe.non_empty_cs > 0 
		&& pe.cs[0]->size == prefs->first_cs_size) {
			char * bt = pe.cs[0]->start;
			char * buffer = prefs->buf;
			for(int i = 0; i < pe.cs[0]->size; i++, bt++) {
				buffer[i] =  (*bt);
			}
			
			MD5 md5 = MD5();
			md5.update(buffer, pe.cs[0]->size);			
			std::string hash = md5.finalize().hexdigest();

			if(hash == prefs->hash) {
				printf("File with specified hash found: %s\n", path.c_str());
			}

			//printf("%s %s\n", path.c_str(), hash.c_str());
	}
	close_pe_file(&pe);
}

void signature_checker(std::string path, prefs_t * prefs) {
	pe_file_t pe;	
	discover_pe_file(path, &pe, 1);
	if(pe.valid && pe.cs_allocated && pe.non_empty_cs > 0 
		&& pe.cs[0]->size == prefs->first_cs_size) {
			char * bt = pe.cs[0]->start;
			char * buffer = prefs->buf;

			// skip bytes
			for(int i = 0; i < prefs->skip_bytes; i++, bt++);

			bool equals = true;
			for(int i = 0; i < prefs->sign_size; i++, bt++) {
				buffer[i] =  (*bt);

				if((buffer[i] & 0xFF) != prefs->sign[i]) {
					equals = false;
					break; // comment this line to print valid signature
				}
			}

			if(equals) {
				printf("File with specified signature found: %s\n", path.c_str());
			}

			// create signature
			/*int count = 0;
			if(path == "C:\\Windows\\System32/at.exe") {
				for(int i = 0; i < prefs->sign_size; i++) {
					printf("0X%2X, ", buffer[i] & 0xFF);
					if(count++ == 6) {
						printf("\\ \n");
						count = 0;
					}
				}
			}*/

			//printf("%s\n", path.c_str());
	}
	close_pe_file(&pe);
}
