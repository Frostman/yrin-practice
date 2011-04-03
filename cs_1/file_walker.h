#ifndef FILE_WALKER_H
#define FILE_WALKER_H

#include <string>

void walk_path(std::string, void (*)(std::string, prefs_t *), prefs_t *);

#endif /* FILE_WALKER_H */