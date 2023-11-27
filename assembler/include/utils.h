#ifndef UTILS_H
#define UTILS_h
#include "token_type.h"
#include <stdio.h> 

token_type str_to_token_type(const char *text);

void check_file(FILE* f, const char* file_name);

#endif
