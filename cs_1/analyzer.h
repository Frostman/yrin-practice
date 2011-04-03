#ifndef ANALYZER_H
#define ANALYZER_H

#include "types.h"

void discover_pe_file(std::string, pe_file_t *);

void close_pe_file(pe_file_t *);

void hashcode_checker(std::string, prefs_t * );

void signature_checker(std::string, prefs_t * );

#endif /* ANALYZER_H */