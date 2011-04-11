#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <windows.h>

#define __DEBUG__OFF

#include "types.h"
#include "sign_util.h"
#include "file_util.h"

int main(int argc, char * argv[]) {	
	std::string file_path = argv[0];

	printf("Before read data. Touch me quikly asd asd asd asd.\n");
	sign_data_t read_data = calc_sign_data(file_path);
	printf("After read data. Touch me quikly asd asd asd asd.\n");

	sign_data_t curr_data = calc_sign_data(file_path);
	
	
	if(!read_data.ready) {
		printf("First run. Appending sign into the file.\n");		

		append_sign_data(file_path, curr_data);

		printf("Sign appended successfully.\n");
	} else {
		printf("Comparing...\n");

		bool changed = false;
		if(read_data.dir != curr_data.dir) {
			printf("Directory changed\n\t(from '%s' to '%s')\n", read_data.dir.c_str(), curr_data.dir.c_str());
			changed = true;
		}

		if(read_data.name != curr_data.name) {
			printf("File name changed\n\t(from '%s' to '%s')\n", read_data.name.c_str(), curr_data.name.c_str());
			changed = true;
		}

		if(read_data.hash_code != curr_data.hash_code) {
			printf("Hash code changed\n\t(from '%s' to '%s')\n", read_data.hash_code.c_str(), curr_data.hash_code.c_str());
			changed = true;
		}

		if(read_data.file_attrs != curr_data.file_attrs) {
			printf("File attributes changed\n\t(from '%s' to '%s')\n", read_data.file_attrs.c_str(), curr_data.file_attrs.c_str());
			changed = true;
		}

		if(!changed) {
			printf("Nothing changed.\n");
		}
	}	

	printf("\nPress any key to continue...");
	_getch();
}
