#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H
#include<stdint.h>
#include "hashmap.h"

// Determines the type of the token, also used to determine dfa_node state

typedef enum token_type
{
	STRINGLIT,     // 0
	STRINGLITERR,  // 1
	IMMEDIATE,     // 2
	IMMEDIATEERR,  // 3
	IDENTIFIER,    // 4
	IDENTIFIERERR, // 5
	ID_KW_LB,      // 6
	ID_KW_LBERR,   // 7
	LABEL,         // 8
	LABELERR,      // 9
	COMMENT,       // 10
	LPAREN,        // 11
	RPAREN,        // 12
	LSQBR,         // 13
	RSQBR,         // 14
	COMA,          // 15
	MINUS,         // 16
	START,         // 17
	ERROR,         // 18
	END,           // 19
	IM_LB,         // 20
	IM_LBERR,      // 21
	ADD,           // 22
	LB,            // 23
	BEQZ,          // 24
	ADDI,          // 25
	J,             // 26
	RET,           // 27
	SB,            // 28
	BGE,           // 29
	SD,            // 30
	MV,            // 31
	CALL,          // 32
	SRAI,          // 33
	SUB,           // 34
	LD,            // 35
	SLLI,          // 36
	LW,            // 37
	BLE,           // 38
	BNEZ,          // 39
	FLD,           // 40
	FSW,           // 41
	LA,            // 42
	BGT,           // 43
	FLW,           // 44
	FADDS,         // 45
	FMULS,         // 46
	FMULSX,        // 47
	FMVS,          // 48
	FLTS,          // 49
	FGTS,          // 50
	FSQRTD,        // 51
	FADDD,         // 52
	FMULD,         // 53
	FSUBD,         // 54
	LI,            // 55
	FMVSX,		   // 56
	RELLBJMP,	   // 57
	SPECIFIER,     // 58
	SPECIFIERERR,  // 59
	RELLABEL,      // 60
	ZERO,
	RA,
	SP,
	GP,
	TP,
	T0,
	T1,
	T2,
	S0,
	FP,
	S1,
	A0,
	A1,
	A2,
	A3,
	A4,
	A5,
	A6,
	A7,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	S10,
	S11,
	T3,
	T4,
	T5,
	T6,
	FT0,
	FT1,
	FT2,
	FT3,
	FT4,
	FT5,
	FT6,
	FT7,
	FS0,
	FS1,
	FA0,
	FA1,
	FA2,
	FA3,
	FA4,
	FA5,
	FA6,
	FA7,
	FS2,
	FS3,
	FS4,
	FS5,
	FS6,
	FS7,
	FS8,
	FS9,
	FS10,
	FS11,
	FT8,
	FT9,
	FT10,
	FT11,
	REGISTER,
	OFFSET,
	CALLPRINTF,
	CALLSCANF,
	CALLCFUNC,
	CALLSTRLEN,
	CALLINTERN,
	COUNT_TOKEN_TYPE
} token_type;

// Token

typedef struct token
{
	token_type type;  // Type of token (ex: CALL)
	const char* text; // Underlying text of token (ex: "call")
} token;

//  Holds a token array 

typedef struct token_array
{
	token* array;  // Token array itself
	uint32_t size; // Size of token array
} token_array;

extern hashmap_t token_hashmap;

void init_kw_hashmap();

#endif

