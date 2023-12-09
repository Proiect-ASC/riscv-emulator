#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>
#include "token_type.h"

// This function takes in a file path, lexes the file and returns a token array
// Ex: token_array tarr = lex_file("./sample_file.txt");

token_array lex_file(const char* file_name);

void parse(token_array *tarr);

#endif
