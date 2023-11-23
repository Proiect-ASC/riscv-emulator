#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>
#define NUMBERS "0123456789"

// Determines the type of the token, also used to determine dfa_node state

typedef enum token_type
{
	STRINGLIT,
	STRINGLITERR,
	IMMEDIATE,
	IMMEDIATEERR,
	IDENTIFIER,
	IDENTIFIERERR,
	ID_KW,
	ID_KW_LBERR,
	LABEL,
	LABELERR,
	COMMENT,
	LPAREN,
	RPAREN,
	LSQBR,
	RSQBR,
	COMA,
	MINUS,
	START,
	ERROR,
	END,
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
