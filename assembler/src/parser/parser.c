#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dfa_edge new_dfa_edge(dfa_node *next, const char* tc, uint8_t all_but)
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
	if(node->edges == NULL)
		node->edges = (dfa_edge *) malloc(sizeof(dfa_edge));
	else
		node->edges = realloc(node->edges, node->edge_count * sizeof(dfa_edge));
	node->edges[node->edge_count - 1] = edge;
}

dfa_node* init_dfa()
{
	dfa_node* dfa;
	dfa = (dfa_node *) malloc(COUNT * sizeof(dfa_node));
	memset(dfa, 0, COUNT * sizeof(dfa_node));
	add_edge(&dfa[START], new_dfa_edge(&dfa[START], "\n ", 0));
	// STRIGNLIT
	add_edge(&dfa[START], new_dfa_edge(&dfa[STRINGLITERR], "\"", 0));
	add_edge(&dfa[STRINGLITERR], new_dfa_edge(&dfa[STRINGLITERR], "\"", 1));
	add_edge(&dfa[STRINGLITERR], new_dfa_edge(&dfa[STRINGLIT], "\"", 0));
	// IMMEDIATE
	add_edge(&dfa[START], new_dfa_edge(&dfa[IMMEDIATEERR], NUMBERS, 0));
	add_edge(&dfa[IMMEDIATEERR], new_dfa_edge(&dfa[IMMEDIATEERR], NUMBERS, 0));
	add_edge(&dfa[IMMEDIATEERR], new_dfa_edge(&dfa[ERROR], "0123456789 ,", 1));
	add_edge(&dfa[IMMEDIATEERR], new_dfa_edge(&dfa[IMMEDIATE], " ,", 0));
	// COMMENT
	add_edge(&dfa[START], new_dfa_edge(&dfa[COMMENT], "#", 0));
	add_edge(&dfa[COMMENT], new_dfa_edge(&dfa[COMMENT], "\n", 1));
	add_edge(&dfa[COMMENT], new_dfa_edge(&dfa[START], "\n", 0));
	// IDENTIDIER
	add_edge(&dfa[START], new_dfa_edge(&dfa[IDENTIFIERERR], "\n", 0));
	add_edge(&dfa[IDENTIFIERERR], new_dfa_edge(&dfa[IDENTIFIERERR], "\n", 0));
	add_edge(&dfa[IDENTIFIERERR], new_dfa_edge(&dfa[IDENTIFIERERR], "\n", 0));
	
	return dfa;
}
