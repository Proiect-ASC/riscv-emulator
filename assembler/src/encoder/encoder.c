#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "utils.h"
#include <stdint.h>
#define MAX_CODE_LENGTH 27

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
		token_type type;
		if(hm_get(&token_hashmap, instruction, (int *) &type) == 0)
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

char *buffer_to_code(void *buffer, size_t size)
{
	char *code = (char *) malloc(size * 8 + 1);
	code[size * 8] = '\0';
	size_t index = 0;
	for(size_t i = 0; i < size; i++)
	{
		uint8_t val = *((uint8_t *) (buffer + i));
		int bit_pos = 7;
		while(bit_pos > -1)
		{
			code[index++] = (val >> bit_pos--) & 1 ? '1' : '0';
		}
	}
	return code;
}

// Write to encoded array

void wtea(char **dest, char *code)
{
	size_t n = strlen(code);
	*dest = (char *) malloc(n + 1);
	strcpy(*dest, code);
}

typedef struct uint16_array
{
	uint16_t *array;
	size_t size;
} uint16_array;

char **tarr_to_encoded(token_array *tarr, char code_table[][MAX_CODE_LENGTH + 1])
{
	uint16_t pc = 0;
	char **encoded_arr = (char **) calloc(tarr->size, sizeof(char *));
	hashmap_t label_hm;
	uint16_t brellabel[101] = {0};
	uint16_array frellabel[101];
	memset(frellabel, 0, sizeof(frellabel));
	for(size_t i = 0; i < tarr->size; i++)
	{
		char *text = (char *) tarr->array[i].text;
		token_type type = tarr->array[i].type;
		size_t n = strlen(text);
		int num_val = 0;
		switch(type)
		{
			case STRINGLIT:
				text[n - 1] = '\0';
				encoded_arr[i] = buffer_to_code(text + 1, n - 2);
				pc += strlen(encoded_arr[i]);
				break;
			case IMMEDIATE:
				num_val = atoi(text);
				encoded_arr[i] = buffer_to_code(&num_val, sizeof(num_val));
				pc += strlen(encoded_arr[i]);
				break;
			case LABEL:
				text[n - 1] = '\0';
				hm_set(&label_hm, text, (int) pc);
				break;
			case IDENTIFIER:
				if(hm_get(&label_hm, text, &num_val) == -1)
				{
					// TODO: Fix this when able
					break;
					fprintf(stderr, "[ERROR] symbol %s not found\n", text);
					exit(1);
				}
				encoded_arr[i] = buffer_to_code(&num_val, sizeof(pc));
				pc += (8 * sizeof(pc));
				break;
			case RELLABEL:
				text[n - 1] = '\0';
				num_val = atoi(text);
				if(num_val < 1 || num_val > 99) 
				{
					fprintf(stderr, "[ERROR] local should represent a value betweeen 1 and 100, got %d\n", num_val);
					exit(1);
				}
				brellabel[num_val] = pc;
				for(size_t i = 1; i < 101; i++)
				{
					if(!frellabel[i].size) continue;
					for(size_t j = 0; j < frellabel[i].size; j++)
					{
						encoded_arr[frellabel[i].array[j]] = buffer_to_code(&pc, sizeof(pc));
					}
					if(frellabel[i].array != NULL) free(frellabel[i].array);
					frellabel[i].array = NULL;
					frellabel[i].size = 0;
				}
				pc += (8 * sizeof(pc));
				break;
			case RELLBJMP:
				if(text[n - 1] == 'b')
				{
					text[n - 1] = '\0';
					num_val = atoi(text);
					if(brellabel[num_val] == 0)
					{
						fprintf(stderr, "[ERROR] local symbol %s not found", text);
						exit(1);
					}
					encoded_arr[i] = buffer_to_code(&brellabel[num_val], sizeof(pc));
				}
				else
				{
					text[n - 1] = '\0';
					num_val = atoi(text);
					frellabel[num_val].array = (uint16_t *) realloc(frellabel[num_val].array, ++frellabel[num_val].size * 2);
					frellabel[num_val].array[frellabel[num_val].size - 1] = i;
				}
				pc += (8 * sizeof(pc));
				break;
			case LPAREN:
			case RPAREN:
			case LSQBR:
			case RSQBR:
			case COMA:
			case SPECIFIER:
				break;
			default:
				wtea(&encoded_arr[i], code_table[type]);
				pc += strlen(code_table[type]);
				break;
		}
	}
	for(size_t i = 1; i < 101; i++)
	{
		if(frellabel[i].size > 0)
		{
			fprintf(stderr, "[ERROR] local symbol %ld not found", i);
			exit(1);
		}
	}
	return encoded_arr;
}

void encode(token_array *tarr, const char *output_file_name, const char *code_table_path)
{
	FILE *f = fopen(output_file_name, "w");
	check_file(f, output_file_name);
	char code_table[COUNT_TOKEN_TYPE][MAX_CODE_LENGTH + 1] = {{0}};
	char str_byte[9];
	str_byte[9] = '\0';
	uint8_t str_byte_index = 0;
	uint8_t byte = 0;

	load_code_table(code_table_path, code_table);
	char **encoded_arr = tarr_to_encoded(tarr, code_table);
	for(uint32_t i = 0; i < tarr->size; i++)
	{
		size_t code_index = 0;
		char *code = encoded_arr[i];
		while(code != NULL && code_index < strlen(code))
		{
			str_byte[str_byte_index++] = code[code_index++];
			if(str_byte_index == 8)
			{
				byte = str_to_byte(str_byte);
				printf("[DEBUG] str_byte is: %s byte is: %x\n", str_byte, byte);
				fwrite(&byte, 1, 1, f);
				str_byte_index = 0;
			}
		}
		if(code != NULL) free(code);
	}
	free(encoded_arr);
	while(str_byte_index % 8 != 0)
		str_byte[str_byte_index++] = '0';
	if(str_byte_index == 8)
	{
		byte = str_to_byte(str_byte);
		fwrite(&byte, 1, 1, f);
		printf("[DEBUG] str_byte is: %s byte is: %x\n", str_byte, byte);
	}
	fclose(f);
}

