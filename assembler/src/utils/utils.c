#include <stdlib.h>
#include <string.h>
#include "utils.h"

token_type str_to_token_type(const char *text)
{
	if(strcmp(text, "add") == 0)
		return ADD;
	else if(strcmp(text, "lb") == 0)
		return LB;
	else if(strcmp(text, "beqz") == 0)
		return BEQZ;
	else if(strcmp(text, "addi") == 0)
		return ADDI;
	else if(strcmp(text, "j") == 0)
		return J;
	else if(strcmp(text, "mv") == 0)
		return MV;
	else if(strcmp(text, "ret") == 0)
		return RET;
	else if(strcmp(text, "sb") == 0)
		return SB;
	else if(strcmp(text, "bge") == 0)
		return BGE;
	else if(strcmp(text, "sd") == 0)
		return SD;
	else if(strcmp(text, "mv") == 0)
		return MV;
	else if(strcmp(text, "call") == 0)
		return CALL;
	else if(strcmp(text, "srai") == 0)
		return SRAI;
	else if(strcmp(text, "sub") == 0)
		return SUB;
	else if(strcmp(text, "ld") == 0)
		return LD;
	else if(strcmp(text, "slli") == 0)
		return SLLI;
	else if(strcmp(text, "lw") == 0)
		return LW;
	else if(strcmp(text, "ble") == 0)
		return BLE;
	else if(strcmp(text, "bnez") == 0)
		return BNEZ;
	else if(strcmp(text, "fld") == 0)
		return FLD;
	else if(strcmp(text, "fsw") == 0)
		return FSW;
	else if(strcmp(text, "la") == 0)
		return LA;
	else if(strcmp(text, "bgt") == 0)
		return BGT;
	else if(strcmp(text, "flw") == 0)
		return FLW;
	else if(strcmp(text, "fadd.s") == 0)
		return FADDS;
	else if(strcmp(text, "fmul.s") == 0)
		return FMULS;
	else if(strcmp(text, "fmul.s.x") == 0)
		return FMULSX;
	else if(strcmp(text, "fmv.s") == 0)
		return FMVS;
	else if(strcmp(text, "flt.s") == 0)
		return FLTS;
	else if(strcmp(text, "fgt.s") == 0)
		return FGTS;
	else if(strcmp(text, "fsqrt.d") == 0)
		return FSQRTD;
	else if(strcmp(text, "fadd.d") == 0)
		return FADDD;
	else if(strcmp(text, "fmul.d") == 0)
		return FMULD;
	else if(strcmp(text, "fsub.d") == 0)
		return FSUBD;
	else if(strcmp(text, "li") == 0)
		return LI;
	else if(strcmp(text, "fmv.s.x") == 0)
		return FMVSX;
	return ERROR;
}

void check_file(FILE* f, const char* file_name)
{
	if(f == NULL)
	{
		fprintf(f, "[ERROR] could not oopen file: %s\n", file_name);
		exit(1);
	}
}


