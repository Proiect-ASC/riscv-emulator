#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "encoder.h"
#include "utils.h"
#define PROG_ARGC 3
#define CODE_TABLE_PATH "./code_table.txt"

int main(int argc, char **argv)
{
	init_kw_hashmap();
	if(argc != PROG_ARGC)
	{
		printf("Usage: <program_binary_name> <input_file> <output_file>");
		exit(1);
	}
	char *in_file_name = argv[1];
	char *out_file_name = argv[2];
	printf("[INFO] Assembling file: %s\n", in_file_name);
	token_array tarr = lex_file(in_file_name);
	parse(&tarr);
	encode(&tarr, out_file_name, CODE_TABLE_PATH); 
	printf("[INFO] Done. Exiting...\n");
	hm_clear(&token_hashmap);
	return 0;
}

