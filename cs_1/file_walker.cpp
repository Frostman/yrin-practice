#include <windows.h>
#include <string>

std::string getFileName(WIN32_FIND_DATA find_data);

bool isExeFile(std::string path);

bool isDir(int file_attrs);

void walk_path(std::string path, void (*file_handler)(std::string)) {
	// folder elements iterator
	WIN32_FIND_DATA find_data;
	HANDLE find_handler;	
	find_handler = FindFirstFile((path + "\\*.*").c_str(), &find_data);
	if (find_handler != INVALID_HANDLE_VALUE){		
		do {
			std::string directory;
			int file_attrs = find_data.dwFileAttributes;
			std::string file_name = getFileName(find_data);
			if ("." == file_name|| ".." == file_name) {
				continue;
			}
			directory = path + "/" + file_name;
			
			if (!isDir(file_attrs) && isExeFile(directory)) {
					file_handler(directory);							
			} else {				
				walk_path(directory, file_handler);				
			}
			// итеририруемся по результатам поиска (find_data)
		} while(FindNextFile(find_handler, &find_data));
		FindClose(find_handler);
	}
}


std::string getFileName(WIN32_FIND_DATA find_data){
	std::string tmp;
	for(int i = 0; i < 260; i++){
		char ch = find_data.cFileName[i];
		if (ch) {
			tmp += ch;
		} else {
			break;
		}
	}

	return tmp;
}

bool isExeFile(std::string path) {
	FILE * file = NULL;
	char buffer[2];
	memset(buffer, 0, 2);

	if((file = fopen(path.c_str(), "rb")) == NULL) {
		return false;
	}

	fread(buffer, sizeof(unsigned char), 2, file);
	bool result = false;
	if(strcmp("MZ", buffer) == 0 || strcmp("ZM", buffer)) {		
		result = true;
	}

	fclose(file);
	return result;
}

bool isDir(int file_attrs) {
	// if mask lt 31 it's dir
	return file_attrs < 31;
}
