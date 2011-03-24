#ifndef FILE_WALKER_H
#define FILE_WALKER_H

#include <string>

void walk_path(std::string, void (*)(std::string));

#endif /* FILE_WALKER_H */