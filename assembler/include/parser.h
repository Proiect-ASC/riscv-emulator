#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>
#define NUMBERS "0123456789"
#define ID_KW_LB_CHARS "_0123456789qwertyuiopasdfghjklzxcvbnm.-"
#define ID_KW_LB_VLDCHARS "_0123456789qwertyuiopasdfghjklzxcvbnm.- \n:"


// Determines the type of the token, also used to determine dfa_node state

typedef enum token_type
{
	STRINGLIT,
	STRINGLITERR,
	IMMEDIATE,
	IMMEDIATEERR,
	IDENTIFIER,
	IDENTIFIERERR,
	ID_KW_LB,
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
	COUNT
} token_type;

// Token

typedef struct token
{
	token_type type;
	const char* text;
} token;

// This structure stores a dfa edge

typedef struct dfa_node dfa_node;

typedef struct dfa_edge
{
	dfa_node *next; // The node it points to
	const char* transition_chars; // The characters it must encounter transition
	uint8_t all_but; // all but transition_chars (1) or default (0)
} dfa_edge;

// This structure represents a dfa node

typedef struct dfa_node
{
	dfa_edge *edges;
	uint8_t edge_count;	
} dfa_node;

dfa_edge new_dfa_edge(dfa_node *next, const char* tc, uint8_t all_but);

void add_edge(dfa_node *node, dfa_edge edge);

struct dfa_node* init_dfa();

token_type transition(dfa_node *node, const char next_ch);

#endif
