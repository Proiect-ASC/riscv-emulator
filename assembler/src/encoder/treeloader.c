#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeloader.h"

codes_map codes;

huffman_node new_huffman_node(const char *name, const char *code, int8_t l, int8_t r)
{
	huffman_node node;
	if(name == NULL)
		node.name = NULL;
	else
	{
		node.name = (const char *) malloc(strlen(name) + 1);
		strcpy((char *) node.name, (char *) name);
	}
	if(code == NULL)
		node.code = NULL;
	else
	{
		node.code = (const char *) malloc(strlen(code) + 1);
		strcpy((char *) node.code, (char *) code);
	}
	node.left_index = l;
	node.right_index = r;
	return node;
}

// Returns last node_index

uint8_t dfs(FILE *f, huffman_node *root, char* code, int code_length, uint8_t node_index)
{
	char line[32];
	if(fgets(line, 32, f) == NULL)
		return node_index;
	char code_char = line[0];
	code[code_length] = code_char;
	code[++code_length] = '\0';
	huffman_node *node = root + node_index;

	// In other words, node is a leaf
	
	if(strlen(line) > 2)
	{
		char *instr_name = line + 2;
		line[strlen(instr_name) - 1] = '\0';
		*node = new_huffman_node(instr_name, code, -1, -1);
		strcpy(codes.map[++codes.size], code);
		return node_index;
	}

	// node is not a leaf
	
	uint8_t left_last_index = dfs(f, root, code, code_length, node_index + 1);
	uint8_t right_last_index = dfs(f, root, code, code_length, left_last_index + 1);
	*node = new_huffman_node(NULL, NULL, node_index + 1, left_last_index + 1);

	return right_last_index;
}

huffman_tree load_huffman_tree(const char* file_name)
{
	FILE *f = fopen(file_name, "r");
	huffman_tree tree;
	char code_buffer[9];

	fscanf(f, "%hhd", &tree.size);
	tree.root = (huffman_node *) malloc(tree.size * sizeof(huffman_node));
	dfs(f, tree.root, code_buffer, 0, 0); 

	fclose(f);
	return tree;
}
