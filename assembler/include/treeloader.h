#ifndef HUFFMAN_TREE_LOADER_H
#define HUFFMAN_TREE_LOADER_H
#include <stdint.h>
#define CODES_MAP_LENGTH 128

typedef struct huffman_node
{
	const char *name;
	const char *code;
	int8_t left_index;
	int8_t right_index;
} huffman_node;

typedef struct huffman_tree
{
	huffman_node* root;
	uint8_t size;
} huffman_tree;

typedef struct codes_map
{
	char map[CODES_MAP_LENGTH][32];
	uint8_t size;
} codes_map;

extern codes_map codes;

huffman_tree load_huffman_tree(const char *file_name);

#endif
