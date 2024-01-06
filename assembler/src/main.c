#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "treeloader.h"
#include "encoder.h"
#include "utils.h"
#define PROG_ARGC 3
#define CODE_TABLE_PATH "../code_table.txt"

// For debug only

/* char token_type_map[][20] = {
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
	"RELLBJMP",
	"SPECIFIER",
	"SPECIFIERERR",
	"RELLABEL",
	"ZERO",
	"RA",
	"SP",
	"GP",
	"TP",
	"T0",
	"T1",
	"T2",
	"S0",
	"FP",
	"S1",
	"A0",
	"A1",
	"A2",
	"A3",
	"A4",
	"A5",
	"A6",
	"A7",
	"S2",
	"S3",
	"S4",
	"S5",
	"S6",
	"S7",
	"S8",
	"S9",
	"S10",
	"S11",
	"T3",
	"T4",
	"T5",
	"T6",
	"FT0",
	"FT1",
	"FT2",
	"FT3",
	"FT4",
	"FT5",
	"FT6",
	"FT7",
	"FS0",
	"FS1",
	"FA0",
	"FA1",
	"FA2",
	"FA3",
	"FA4",
	"FA5",
	"FA6",
	"FA7",
	"FS2",
	"FS3",
	"FS4",
	"FS5",
	"FS6",
	"FS7",
	"FS8",
	"FS9",
	"FS10",
	"FS11",
	"FT8",
	"FT9",
	"FT10",
	"FT11",
	"REGISTER",
	"OFFSET"
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
} */

int main(int argc, char **argv)
{
	init_kw_hashmap();
	// For debug only
	/*for(size_t i = 0; i < 12; i++)
	{
		char file_name[200];
		char extension[] = ".txt";
		char folder_out[] = "/home/tiberiu/Documents/C/riscv-emulator/assembler/example_binaries/";
		char folder_in[] = "/home/tiberiu/Documents/C/riscv-emulator/assembler/examples/";
		memcpy(file_name, folder_in, sizeof(folder_in));
		if(i + 1 > 9) file_name[sizeof(folder_in)-1] = 'A' + i - 9;
		else file_name[sizeof(folder_in) - 1] = '1' + i;
		strcpy(file_name + sizeof(folder_in), extension);
		printf("[DEBUG] assembling file %s...", file_name);
		token_array tarr = lex_file(file_name);
		parse(&tarr);
		//print_token_array(tarr);
		char output_file[200];
		memcpy(output_file, folder_out, sizeof(folder_out));
		output_file[sizeof(folder_out) - 1] = i + 1 > 9 ? 'A' + i - 9 : '1' + i;
		strcpy(output_file + sizeof(folder_out), extension);
		encode(&tarr, output_file, "code_table.txt");
		printf("[DEBUG] Done\n\n");
	}*/
	if(argc != PROG_ARGC)
	{
		fprintf(stderr, "[ERROR] expected %d program arguments, got %d instead\n", PROG_ARGC, argc);
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

