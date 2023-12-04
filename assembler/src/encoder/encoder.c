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

typedef struct bytearray_t
{
	uint8_t *array;
	size_t size;
} bytearray_t;

void write_buffer(bytearray_t *dest, void *src, int n)
{
	dest->array = (uint8_t *) malloc(n);
	memcpy(dest->array, src, n);
	dest->size = n;
}

void tarr_to_encoded(bytearray_t *encoded_arr, token_array tarr, char code_table[][MAX_CODE_LENGTH + 1])
{
	uint16_t address = 0;
	hashmap_t ident_hm;
	hashmap_t rel_labels_hm;
	int buffer;
	for(size_t i = 0; i < tarr.size; i++)
	{
		char *text = (char *) tarr.array[i].text;
		token_type type = tarr.array[i].type;
		size_t instruction_length = 0;
		switch(type)
		{
			case LABEL:
				text[strlen(text) - 1] = '\0';
				if(hm_get(&ident_hm, text, &buffer) == 0 && buffer != address)
				{
					fprintf(stderr, "[ERROR] symbol '%s' is already defined", text);
					exit(1);
				}
				hm_set(&ident_hm, text, address);
				text[strlen(text)] = ':';
				break;
			case RELLABEL:
				buffer = (int) text[strlen(text)];
				text[strlen(text)] = '\0';
				hm_set(&rel_labels_hm, text, address);
				text[strlen(text)] = (char) buffer;	
				break;
			case RELLBJMP:
				if(text[strlen(text) - 2] == 'b')
				{
					text[strlen(text) - 1] = '\0';
					if(hm_get(&rel_labels_hm, text, &buffer) == -1)
					{
						fprintf(stderr, "[ERROR] symbol not found: %s", text);
						exit(1);
					}
					write_buffer(&encoded_arr[i], &buffer, 2);
					text[strlen(text)] = 'b';
				}
				instruction_length = 16;
				break;
			case IDENTIFIER:
				if(hm_get(&ident_hm, text, &buffer) == 0)
				{
					write_buffer(&encoded_arr[i], &buffer, 2);
					instruction_length = 16;
				}
				break;
			case STRINGLIT:
				write_buffer(&encoded_arr[i], text + 1, strlen(text) - 2);
				instruction_length = strlen(text) - 2;
				break;
			case IMMEDIATE:
				// TODO: This should allocate memory depending on context, this is a placeholder
				buffer = atoi(text);
				write_buffer(&encoded_arr[i], &buffer, 4);
				break;
			default:
				instruction_length += strlen(code_table[type]);
				break;
		}
		if(address + instruction_length < address)
		{
			perror("[ERROR] maximum binary length exeeded");
			exit(1);
		}
		address += instruction_length;
	}
	hm_clear(&rel_labels_hm);
	for(int i = tarr.size; i >= 0; i--)
	{
		token_type type = tarr.array[i].type;
		char *text = (char *) tarr.array[i].text;
		if(type == RELLBJMP && text[strlen(text) - 1] == 'f')
		{
			text[strlen(text) - 1] = '\0';
			if(hm_get(&rel_labels_hm, text, &buffer) == -1)
			{
				fprintf(stderr, "[ERROR] symbol not found: %s", text);
				exit(1);
			}
			write_buffer(&encoded_arr[i], &buffer, 2);
			text[strlen(text)] = 'f';
		}
		else if(type == RELLABEL)
		{
			buffer = (int) text[strlen(text)];
			text[strlen(text)] = '\0';
			hm_set(&rel_labels_hm, text, address);
			text[strlen(text)] = (char) buffer;
		}
	}
	hm_clear(&ident_hm);
	hm_clear(&rel_labels_hm);
}

void encode(token_array tarr, const char *output_file_name, const char *code_table_path)
{
	FILE *f = fopen(output_file_name, "w");
	check_file(f, output_file_name);
	char code_table[COUNT_TOKEN_TYPE][MAX_CODE_LENGTH + 1] = {{0}};
	char str_byte[9];
	str_byte[9] = '\0';
	uint8_t str_byte_index = 0;
	uint8_t byte = 0;
	
	load_code_table(code_table_path, code_table);
	bytearray_t encoded_arr[tarr.size];
	memset(encoded_arr, 0, sizeof(encoded_arr));
	tarr_to_encoded(encoded_arr, tarr, code_table);
	for(uint32_t i = 0; i < tarr.size; i++)
	{
		char *code = NULL;
		size_t code_index = 0;
		token_type type = tarr.array[i].type;
		char *text = (char *) tarr.array[i].text;
		if(hm_get(&token_hashmap, text, NULL) == 0)
		{
			code = (char *) malloc(strlen(code_table[type]) + 1);
			strcpy(code, code_table[type]);
		}
		if(encoded_arr[i].array != NULL)
		{
			code = (char *) malloc(encoded_arr[i].size * 8 + 1);
			code[encoded_arr[i].size * 8] = '\0';
			for(size_t j = 0; j < encoded_arr[i].size; j++)
			{
				int index = 7;
				while(index >= 0)
				{
					code[code_index++] = '0' + ((encoded_arr[i].array[j] >> index) & 1);
					index--;
				}
			}
		}
		code_index = 0;
		while(code != NULL && code_index < strlen(code))
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
		free(code);
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

