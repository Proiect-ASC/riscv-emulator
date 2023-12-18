#ifndef UTILS_H
#define UTILS_h
#include "token_type.h"
#include "hashmap.h"
#include <stdio.h> 

int is_immediate(char *text);

void check_file(FILE* f, const char* file_name);

#endif
