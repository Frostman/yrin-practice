#ifndef SIGN_UTIL_H
#define SIGN_UTIL_H

#include <string>

#include "types.h"

sign_data_t calc_sign_data(std::string);

sign_data_t read_sign_data(std::string);

void append_sign_data(std::string, sign_data_t);

#endif