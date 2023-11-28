#include "token_type.h"
#include <string.h>

hashmap_t token_hashmap;

void init_kw_hashmap()
{
	kvpair_t pairs[] = 
	{
		{ "li", LI },
		{ "add", ADD },
		{ "lb", LB },
		{ "beqz", BEQZ },
		{ "addi", ADDI },
		{ "j", J },
		{ "mv", MV },
		{ "ret", RET },
		{ "sb", SB },
		{ "bge", BGE },
		{ "sd", SD },
		{ "call", CALL },
		{ "srai", SRAI },
		{ "sub", SUB },
		{ "ld", LD },
		{ "slli", SLLI },
		{ "lw", LW },
		{ "ble", BLE },
		{ "bnez", BNEZ },
		{ "fld", FLD },
		{ "fsw", FSW },
		{ "la", LA },
		{ "bgt", BGT },
		{ "flw", FLW },
		{ "fadd.s", FADDS },
		{ "fmul.s", FMULS },
		{ "fmv.s", FMVS },
		{ "fmv.s.x", FMVSX },
		{ "flt.s", FLTS },
		{ "fgt.s", FGTS },
		{ "fsqrt.d", FSQRTD },
		{ "fadd.d", FADDD },
		{ "fmul.d", FMULD },
		{ "fsub.d", FSUBD },

	};
	token_hashmap = init_hashmap(pairs, sizeof(pairs) / sizeof(kvpair_t)); 
}


