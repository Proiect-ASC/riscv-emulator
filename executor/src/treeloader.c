#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeloader.h"

huffman_node new_huffman_node(int value, const char *code, int8_t l, int8_t r)
{
    huffman_node node;
    node.value = value;
    if (code == NULL)
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

uint8_t dfs(FILE *f, huffman_node *root, char *code, int code_length, uint8_t node_index, int *indices, int *index)
{
    char line[32];
    if (fgets(line, 32, f) == NULL)
        return node_index;
    char code_char = line[0];
    code[code_length] = code_char;
    code[++code_length] = '\0';
    huffman_node *node = root + node_index;

    // In other words, node is a leaf

    if (strlen(line) > 2)
    {
        *node = new_huffman_node(indices[(*index)++], code, -1, -1);
        return node_index;
    }

    // node is not a leaf

    uint8_t left_last_index = dfs(f, root, code, code_length, node_index + 1, indices, index);
    uint8_t right_last_index = dfs(f, root, code, code_length, left_last_index + 1, indices, index);
    *node = new_huffman_node(-1, NULL, node_index + 1, left_last_index + 1);

    return right_last_index;
}

huffman_tree load_huffman_tree(const char *file_name, int *indices)
{
    FILE *f = fopen(file_name, "r");
    huffman_tree tree;
    char code_buffer[MAX_CODE_LENGHT];

    fscanf(f, "%hhd", &tree.size);
    tree.nodes = (huffman_node *) malloc(tree.size * sizeof(huffman_node));
    int index = 0;
    dfs(f, tree.nodes, code_buffer, 0, 0, indices, &index);

    fclose(f);
    return tree;
}
