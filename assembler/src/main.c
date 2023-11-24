#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

void print_token_array(token_array arr)
{
	for(uint32_t i = 0; i < arr.size; i++)
	{
		printf("%d: %s\n", arr.array[i].type, arr.array[i].text);
	}
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		perror("[ERROR] there should be exactly one parameter");
		exit(1);
	}
	const char* file_name = argv[1];
	token_array tarr = lex_file(file_name);
	print_token_array(tarr);
}

