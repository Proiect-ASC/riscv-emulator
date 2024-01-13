#ifndef HUFFMAN_TREE_LOADER_H
#define HUFFMAN_TREE_LOADER_H

#include <stdint.h>
#include <stdbool.h>
#include <huffman_enums.h>

#define MAX_CODE_LENGHT 32

typedef union tree_val {
    int i;
    void *ptr;
} tree_val;

typedef struct huffman_node {
    tree_val value;
    const char *code;
    int8_t left_index;
    int8_t right_index;
} huffman_node;

typedef struct huffman_tree {
    huffman_node *nodes;
    uint8_t size;
} huffman_tree;

huffman_tree load_huffman_tree_instr(const char *file_name, instruction *indices);

huffman_tree load_huffman_tree_reg(const char *file_name, rgstr *indices);

#endif
