#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ID_KW_LB_STARTCHARS "_0123456789qwertyuiopasdfghjklzxcvbnm"
#define ID_KW_LB_VLDCHARS "_0123456789qwertyuiopasdfghjklzxcvbnm.- \n:"

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
	add_edge(&dfa[LABELERR], new_dfa_edge(ERROR, ID_KW_LB_VLDCHARS, 1));
	add_edge(&dfa[LABELERR], new_dfa_edge(LABEL, " \n", 0));
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(LABEL, ":", 0));
	// ID_KW
	add_edge(&dfa[START], new_dfa_edge(ID_KW_LBERR, ID_KW_LB_STARTCHARS, 0));
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(ERROR, ID_KW_LB_VLDCHARS, 0));
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(ID_KW, " ,\n", 0));
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
		if(strchr(edge.transition_chars, (int) ch) != NULL)
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
	strcpy(t.text, text);
	return t;
}

int is_terminal(token_type state)
{
	return state == LABEL || state == COMA || state == MINUS ||
		   state == LPAREN || state == RPAREN || state == LSQBR ||
		   state == RSQBR || state == ID_KW || state == STRINGLIT;
}

token_array lex_file(const char* file_name)
{
	FILE* f = fopen(file_name, "r");
	char curr_char = "\0";
	token_type curr_state = START;
	const char word[64];
	int word_len = 0;
	token_array tarr;
	tarr.array = NULL;
	tarr.size = 0;
	
	while(1)
	{
		fscanf(f, "%c", &curr_char);
		if(curr_char == EOF)
		{
			tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));	
			tarr.array[tarr.size - 1] = new_token(END, "EOF"); 
			break;
		}
		if(is_terminal(curr_state))
		{
			tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));
			word[word_len] = '\0';
			tarr.array[tarr.size - 1] = new_token(next_state, word);
			word_len = 0;
			curr_state = transition(START, curr_char);
		}
		else
		{
			word[word_len++] = curr_char;	
			if(word_len > 63)
			{
				perror("[ERROR] word buffer overflow");
				exit(1);
			}		
		}
		curr_state = transition(curr_state, ch);
	}
	return tarr;
}








