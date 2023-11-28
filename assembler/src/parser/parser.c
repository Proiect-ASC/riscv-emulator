#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#define LETTERS  "qwertyuiopasdfghjklzxcvbnm"
#define NUMBERS "0123456789"
#define LETANDNUM "qwertyuiopasdfghjklzxcvbnm0123456789"
#define KWIDLB "qwertyuiopasdfghjklzxcvbnm0123456789._-"
#define SEPARATORS " \n,()[]-"

typedef struct dfa_edge
{
	token_type next; // The node it points to
	const char* transition_chars; // The characters it must encounter transition
	uint8_t all_but; // all but transition_chars (1) or default (0)
} dfa_edge;

typedef struct dfa_node
{
	dfa_edge *edges;
	uint8_t edge_count;	
} dfa_node;

dfa_edge new_dfa_edge(token_type next, const char* tc, uint8_t all_but)
{
	dfa_edge edge;
	edge.next = next;
	edge.transition_chars = tc;
	edge.all_but = all_but;
	return edge;
}

void add_edge(dfa_node *node, dfa_edge edge)
{
	node->edge_count++;
	node->edges = (dfa_edge *) realloc(node->edges, node->edge_count * sizeof(dfa_edge));
	node->edges[node->edge_count - 1] = edge;
}

dfa_node* init_dfa()
{
	dfa_node* dfa;
	dfa = (dfa_node *) malloc(COUNT_TOKEN_TYPE * sizeof(dfa_node));
	memset(dfa, 0, COUNT_TOKEN_TYPE * sizeof(dfa_node));
	add_edge(&dfa[START], new_dfa_edge(START, "\n ", 0));
	// STRIGNLIT
	add_edge(&dfa[START], new_dfa_edge(STRINGLITERR, "\"", 0));
	add_edge(&dfa[STRINGLITERR], new_dfa_edge(STRINGLIT, "\"", 0));
	// COMMENT
	add_edge(&dfa[START], new_dfa_edge(COMMENT, "#", 0));
	add_edge(&dfa[COMMENT], new_dfa_edge(START, "\n", 0));
	// LABEL
	add_edge(&dfa[START], new_dfa_edge(LABELERR, ".", 0));
	add_edge(&dfa[LABELERR], new_dfa_edge(LABEL, SEPARATORS, 0));
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(LABEL, ":", 0));
	// IM_LB
	add_edge(&dfa[START], new_dfa_edge(IM_LBERR, NUMBERS, 0));
	add_edge(&dfa[IM_LBERR], new_dfa_edge(IM_LB, SEPARATORS, 0));
	// ID_KW_LB
	add_edge(&dfa[START], new_dfa_edge(ID_KW_LBERR, KWIDLB, 0));
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(ID_KW_LB, SEPARATORS, 0));
	// COMA
	add_edge(&dfa[START], new_dfa_edge(COMA, ",", 0));
	// LPAREN
	add_edge(&dfa[START], new_dfa_edge(LPAREN, "(", 0));
	// RPAREN
	add_edge(&dfa[START], new_dfa_edge(RPAREN, ")", 0));
	// LSQBR
	add_edge(&dfa[START], new_dfa_edge(LSQBR, "[", 0));
	// RSQBR
	add_edge(&dfa[START], new_dfa_edge(RSQBR, "]", 0));
	// MINUS
	add_edge(&dfa[START], new_dfa_edge(MINUS, "-", 0));
	
	return dfa;
}

token_type transition(token_type current, const char ch, dfa_node *dfa)
{
	for(int i = 0; i < dfa[current].edge_count; i++)
	{
		dfa_edge edge = dfa[current].edges[i];
		if(strchr(edge.transition_chars, (int) ch) != NULL && edge.all_but == 0)
			return edge.next;
		else if(strchr(edge.transition_chars, (int) ch) == NULL && edge.all_but == 1)
			return edge.next;
	}
	if(current == START)
		return ERROR;
	return current;
}

token new_token(token_type type, const char* text)
{
	token t;
	t.type = type;
	t.text = (const char *) malloc(strlen(text) + 1);
	strcpy((char *) t.text, text);
	return t;
}

token_type get_kw_or_id(const char* text)
{
	token_type token;
	if(hm_get(&token_hashmap, text, (int *) &token) == 0)
		return token;
	return IDENTIFIER;
}

token_type get_token_type(token_type state, const char* text)
{
	token_type type;
	switch(state)
	{
		case ID_KW_LB:
			if(text[strlen(text) - 1] == ':')
				type = LABEL;
			else
				type = get_kw_or_id(text);
			break;
		case IM_LB:
			if(text[strlen(text) - 1] == ':')
				type = LABEL;
			else
				type = IMMEDIATE;
			break;
		default:
			type = state;
			break;
	}
	return type;
}

token_array lex_file(const char* file_name)
{
	FILE* f = fopen(file_name, "r");
	char curr_char = '\0';
	token_type curr_state = START;
	int word_buffer_len = 256;
	char word[word_buffer_len];
	int word_len = 0;
	dfa_node *dfa = init_dfa();
	token_array tarr;
	tarr.array = NULL;
	tarr.size = 0;
	
	while(1)
	{
		int input = fgetc(f);
		if(input == EOF)
		{
			tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));	
			tarr.array[tarr.size - 1] = new_token(END, "EOF"); 
			break;
		}
		curr_char = (char) input;
		curr_state = transition(curr_state, curr_char, dfa);
		switch(curr_state)
		{
			case LPAREN:
			case RPAREN:
			case LSQBR:
			case RSQBR:
			case MINUS:
			case COMA:
			case ID_KW_LB:
			case IM_LB:
				tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));
				word[word_len] = '\0';
				tarr.array[tarr.size - 1] = new_token(get_token_type(curr_state, word), word);
				curr_state = transition(START, curr_char, dfa);
				word_len = 0;
				if(curr_char != ' ' && curr_char != '\n')
				{
					word_len = 1;
					word[0] = curr_char;
				}
				break;
			case STRINGLIT:
			case LABEL:
				tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token)); 
				if(curr_char != ' ' && curr_char != '\n')
					word[word_len++] = curr_char;
				word[word_len] = '\0';
				tarr.array[tarr.size - 1] = new_token(get_token_type(curr_state, word), word);
				curr_state = START;
				word_len = 0;
				break;
			case START:
			case COMMENT:
				word_len = 0;
				word[0] = curr_char;
				break;
			case STRINGLITERR:
			case ID_KW_LBERR:
			case LABELERR:
			case IM_LBERR:
				word[word_len++] = curr_char;	
				if(word_len == word_buffer_len)
				{
					perror("[ERROR] word buffer overflow\n");
					exit(1);
				}		
				break;
			default:
				fprintf(stderr, "[ERROR] undefined behavior: encountered %d\n", curr_state);
				exit(1);
				break;
				
		}
	}
	free(dfa);
	fclose(f);
	return tarr;
}

