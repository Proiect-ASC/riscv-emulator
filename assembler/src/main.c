#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "treeloader.h"
#include "encoder.h"
#include "utils.h"

// For debug only

char token_type_map[][20] = {
	"STRINGLIT",
	"STRINGLITERR",
	"IMMEDIATE",
	"IMMEDIATEERR",
	"IDENTIFIER",
	"IDENTIFIERERR",
	"ID_KW_LB",
	"ID_KW_LBERR",
	"LABEL",
	"LABELERR",
	"COMMENT",
	"LPAREN",
	"RPAREN",
	"LSQBR",
	"RSQBR",
	"COMA",
	"MINUS",
	"START",
	"ERROR",
	"END",
	"IM_LB",
	"IM_LBERR",
	"ADD",
	"LB",
	"BEQZ",
	"ADDI",
	"J",
	"RET",
	"SB",
	"BGE",
	"SD",
	"MV",
	"CALL",
	"SRAI",
	"SUB",
	"LD",
	"SLLI",
	"LW",
	"BLE",
	"BNEZ",
	"FLD",
	"FSW",
	"LA",
	"BGT",
	"FLW",
	"FADDS",
	"FMULS",
	"FMULSX",
	"FMVS",
	"FLTS",
	"FGTS",
	"FSQRTD",
	"FADDD",
	"FMULD",
	"FSUBD",
	"LI",
	"FMVSX",
	"RELLABEL"
};

void print_token_array(token_array tarr)
{
	printf("\n[DEBUG] printing token array\n");
	for(uint32_t i = 0; i < tarr.size; i++)
	{
		printf("%s: %s\n", token_type_map[tarr.array[i].type], tarr.array[i].text);
	}
}

void print_token_hashmap()
{
	printf("\n[DEBUG] printing token hashmap\n\n");
	for(uint32_t i = 0; i < HM_CAPACITY; i++)
	{
		if(token_hashmap.entries[i].size != 0)
		{
			hmentry_t *entry = &token_hashmap.entries[i];
			for(uint16_t j = 0; j < entry->size; j++)
			{
				printf("key: %s\nvalue: %d\n\n", entry->data[j].key, entry->data[j].value);
			}
		}
	}
}

int main(int argc, char **argv)
{
	init_kw_hashmap();
	if(argc != 2)
	{
		perror("[ERROR] there should be exactly one parameter");
		exit(1);
	}
	const char* file_name = argv[1];
	token_array tarr = lex_file(file_name);
	print_token_array(tarr);
	encode(tarr, "output.txt", "code_table.txt");
	// print_token_hashmap();
}

