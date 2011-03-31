#ifndef ANALYZER_H
#define ANALYZER_H

void discover_pe_file(std::string, pe_file_t *);

void close_pe_file(pe_file_t *);

void hashcode_checker(std::string, void * );

void signature_checker(std::string, void * );

#endif /* ANALYZER_H */