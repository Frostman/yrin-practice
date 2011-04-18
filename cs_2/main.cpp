#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <windows.h>
#include <sstream>

#define __DEBUG__OFF

#include "types.h"
#include "sign_util.h"
#include "file_util.h"

int main(int argc, char * argv[]) {	
	std::string file_path = argv[0];
	
	sign_data_t read_data = read_sign_data(file_path);
	sign_data_t curr_data = calc_sign_data(file_path);
	
	if(!read_data.ready) {
		printf("First run. Appending sign into the file.\n");		

		append_sign_data(file_path, curr_data);

		printf("Sign appended successfully.\n");
	} else {
		printf("Comparing...\n");

		std::stringstream out;
		bool changed = false;
		if(read_data.dir != curr_data.dir) {
			out << "Directory changed\n\t(from '" << read_data.dir
				<< "' to '"<< curr_data.dir
				<< "')\n";
			changed = true;
		}

		if(read_data.name != curr_data.name) {
			out << "File name changed\n\t(from '" << read_data.name
				<< "' to '" << curr_data.name
				<< "')\n";
			changed = true;
		}

		if(read_data.hash_code != curr_data.hash_code) {
			out << "Hash code changed\n\t(from '" << read_data.hash_code
				<< "' to '" << curr_data.hash_code
				<< "')\n";
			changed = true;
		}

		if(read_data.file_attrs != curr_data.file_attrs) {
			out << "File attributes changed\n\t(from '" << read_data.file_attrs
				<< "' to '" << curr_data.file_attrs
				<< "')\n";
			changed = true;
		}

		if(!changed) {
			printf("Nothing changed.\n");
		} else {
			printf("Changes:\n%s", out.str().c_str());
		}
	}	

	printf("\nPress any key to continue...");
	_getch();
}
