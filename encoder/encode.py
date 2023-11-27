from data_structs.priorityqueue import PriorityQueue
from data_structs.btree import Node
from stats import read_tokens, count_tokens, compute_appearence_entropy, compute_appearence_probabilities
from huffman import build_huffman_tree
from huffman_tree_serializer import serialize_huffman_tree
from code_table_serializer import serialize_code_table


def encoding_efficiency_test(code_table, tokens_counts, entropy):
    tokens_probs = {k: (lambda x: x / sum(list(tokens_counts.values())))(v) for k, v in tokens_counts.items()}
    average_encoding_lenght = 0
    for item, code in code_table.items():
        average_encoding_lenght += len(code) * tokens_probs[item]
    return entropy / average_encoding_lenght * 100, average_encoding_lenght


tokens = read_tokens("instructions.txt")
tokens_counts = count_tokens("raw_data.txt", tokens)
tokens_probs = compute_appearence_probabilities(tokens_counts)
entropy = compute_appearence_entropy(tokens_probs)

tokens_counts_nodes_pq = PriorityQueue([Node(value=[v, k]) for k, v in tokens_counts.items()])

huffman_tree, code_table, node_count = build_huffman_tree(tokens_counts_nodes_pq)
print(f"encoding eff. is {encoding_efficiency_test(code_table, tokens_counts, entropy)}, code table is:\n{code_table}")

serialize_huffman_tree(huffman_tree, "serialized_huffman_tree.txt", node_count)
serialize_code_table(code_table, "serialized_code_table.txt")
