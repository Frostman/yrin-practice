#ifndef TYPES_H
#define TYPES_H

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


#endif /* TYPES_H */