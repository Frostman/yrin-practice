#ifndef PROCESS_UTIL_H
#define PROCESS_UTIL_H

#include <string>

int is_process_runned(std::string);

void run_process(std::string, std::string, std::string);

#endif /* PROCESS_UTIL_H */