#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "utils.h"
#include <stdint.h>
#define MAX_CODE_LENGTH 8

void load_code_table(const char *file_name, char table[][MAX_CODE_LENGTH + 1])
{
	FILE* f = fopen(file_name, "r");
	check_file(f, file_name);
	char buffer[64];
	while(fgets(buffer, 64, f) != NULL)
	{
		char *instruction = strtok(buffer, " ");
		char *code = strtok(NULL, " ");
		code[strlen(code) - 1] = '\0';
		token_type type = str_to_token_type(instruction);
		strcpy(table[type], code);
		//printf("[DEBUG] table[%s(%d)]: %s\n", instruction, type, code);
	}
}

uint8_t str_to_byte(char *str)
{
	if(strlen(str) != 8)
	{
		perror("[ERROR] str_to_byte: strlen of str should be 8");
		exit(1);
	}
	int p = 1 << 7;
	uint8_t byte = 0;
	for(uint32_t i = 0; i < 8; i++)
	{
		if(str[i] == '1')
			byte += p;
		p >>= 1;
	}
	return byte;
}

void encode(token_array tarr, const char *output_file_name, const char *code_table_path)
{
	FILE *f = fopen(output_file_name, "w");
	check_file(f, output_file_name);
	char code_table[COUNT_TOKEN_TYPE][MAX_CODE_LENGTH + 1] = {{0}};
	char str_byte[9];
	str_byte[9] = '\0';
	uint32_t str_byte_index = 0;
	uint8_t byte;
	
	load_code_table(code_table_path, code_table);
	for(uint32_t i = 0; i < tarr.size; i++)
	{
		char *code = code_table[tarr.array[i].type];
		if(*code == '\0') continue; // PLACEHOLDER
		uint32_t code_index = 0;
		while(code_index < strlen(code))
		{
			str_byte[str_byte_index++] = code[code_index++];
			if(str_byte_index == 8)
			{
				byte = str_to_byte(str_byte);
				printf("[DEBUG] str_byte is: %s   byte is: %x\n", str_byte, byte);
				fwrite(&byte, 1, 1, f);
				str_byte_index = 0;
			}
		}
	}
	while(str_byte_index % 8 != 0)
		str_byte[str_byte_index++] = '0';
	if(str_byte_index == 8)
	{
		byte = str_to_byte(str_byte);	
		fwrite(&byte, 1, 1, f);
		printf("[DEBUG] str_byte is: %s   byte is: %x\n", str_byte, byte);
	}
}

