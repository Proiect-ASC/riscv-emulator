#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#define LETTERS  "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
#define NUMBERS "0123456789"
#define LETANDNUM "qwertyuiopasdfghjklzxcvbnm0123456789QWERTYUIOPASDFGHJKLZXCVBNM"
#define KWIDLBSTART "qwertyuiopasdfghjklzxcvbnm0123456789_QWERTYUIOPASDFGHJKLZXCVBNM-"
#define KWIDLB "qwertyuiopasdfghjklzxcvbnm0123456789._-QWERTYUIOPASDFGHJKLZXCVBNM"
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
	// SPECIFIER
	add_edge(&dfa[START], new_dfa_edge(SPECIFIERERR, ".", 0));
	add_edge(&dfa[SPECIFIERERR], new_dfa_edge(SPECIFIER, SEPARATORS, 0));
	// LABEL
	add_edge(&dfa[ID_KW_LBERR], new_dfa_edge(LABEL, ":", 0));
	// ID_KW_LB
	add_edge(&dfa[START], new_dfa_edge(ID_KW_LBERR, KWIDLBSTART, 0));
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

token_type get_kw_or_id(char* text)
{
	token_type type;
	if(hm_get(&token_hashmap, text, (int *) &type) == 0)
		return type;
	type = IDENTIFIER;
	size_t n = strlen(text);
	char temp = text[n - 1];
	text[n - 1] = '\0';
	if(is_immediate(text) && (temp == 'f' || temp == 'b'))
		type = RELLBJMP;
	text[n - 1] = temp;
	return type;
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
				type = get_kw_or_id((char *) text);
			break;
		default:
			type = state;
			break;
	}
	return type;
}

token_type label_or_rellabel(char *text)
{
	token_type ans = LABEL;
	text[strlen(text) - 1] = '\0';
	if(is_immediate(text)) ans = RELLABEL;
	text[strlen(text)] = ':';
	return ans;
}

token_array lex_file(const char* file_name)
{
	FILE* f = fopen(file_name, "r");
	if(f == NULL)
	{
		printf("[ERROR] could not open file %s\n", file_name);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = (char *) malloc(fsize + 1);
	fread(buffer, fsize, 1, f);
	buffer[fsize] = '\0';
	
	char curr_char = '\0';
	token_type curr_state = START;
	int word_buffer_len = 256;
	char word[word_buffer_len];
	int word_len = 0;
	dfa_node *dfa = init_dfa();
	token_array tarr;
	tarr.array = NULL;
	tarr.size = 0;
	
	for(size_t i = 0; i < fsize; i++)
	{
		curr_char = buffer[i];
		curr_state = transition(curr_state, curr_char, dfa);
		switch(curr_state)
		{
			case LPAREN:
			case RPAREN:
			case LSQBR:
			case RSQBR:
			case COMA:
				word[word_len++] = curr_char;
				word[word_len] = '\0';
				tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));
				tarr.array[tarr.size - 1] = new_token(get_token_type(curr_state, word), word);
				word_len = 0;
				curr_state = START;
				break;
			case ID_KW_LB:
			case SPECIFIER:
			case IMMEDIATE:
				tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token));
				word[word_len] = '\0';
				tarr.array[tarr.size - 1] = new_token(get_token_type(curr_state, word), word);
				word_len = 0;
				curr_state = START;
				i--;
				break;
			case STRINGLIT:
			case LABEL:
				word[word_len++] = curr_char;
				word[word_len] = '\0';
				if(curr_state == LABEL) curr_state = label_or_rellabel(word);
				tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token)); 
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
			case SPECIFIERERR:
			case IM_LBERR:
			case IMMEDIATEERR:
				word[word_len++] = curr_char;	
				if(curr_state == STRINGLITERR && word_len > 1 && word[word_len - 2] == '\\')
				{
					word_len--;
					switch(curr_char)
					{
						case 'n':
							word[word_len - 1] = '\n';
							break;
						case '\\':
							word[word_len - 1] = '\\';
							break;
						case 't':
							word[word_len - 1] = '\t';
							break;
						case '\'':
							word[word_len - 1] = '\'';
							break;
						case '\"':
							word[word_len - 1] = '\"';
							break;
						default:
							fprintf(stderr, "[ERROR] invalid character \\%c\n", curr_char);
							exit(1);
					}
				}
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
	tarr.array = (token *) realloc(tarr.array, ++tarr.size * sizeof(token)); 
	tarr.array[tarr.size - 1] = new_token(END, "EOF");
	free(dfa);
	fclose(f);
	return tarr;
}

