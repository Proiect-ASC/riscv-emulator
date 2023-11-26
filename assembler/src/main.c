#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

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
	"FMVSX
};

void print_token_array(token_array arr)
{
	for(uint32_t i = 0; i < arr.size; i++)
	{
		printf("%s: %s\n", token_type_map[arr.array[i].type], arr.array[i].text);
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

