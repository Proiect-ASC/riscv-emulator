#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

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
	COUNT_TOKEN_TYPE
} token_type;

// Token

typedef struct token
{
	token_type type;
	const char* text;
} token;

typedef struct token_array
{
	token* array;
	uint32_t size;
} token_array;

token_array lex_file(const char* file_name);

#endif
