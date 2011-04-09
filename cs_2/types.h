#ifndef TYPES_H
#define TYPES_H

#include <string>

typedef struct sign_data_s {
	boolean ready;

	std::string dir;
	std::string name;
	std::string hash_code;
	std::string file_attrs;
} sign_data_t;

#endif /* TYPES_H */