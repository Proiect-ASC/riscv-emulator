from data_structs.priorityqueue import PriorityQueue
from data_structs.btree import Node


def build_huffman_tree(tokens_counts_nodes_pq):
    while not tokens_counts_nodes_pq.qsize() == 1:
        l, r = tokens_counts_nodes_pq.pop(), tokens_counts_nodes_pq.pop()
        new_node = Node(l, r, val=list(map(lambda x, y: x + y, l.value, r.value)))
        tokens_counts_nodes_pq.push(new_node)

    current = tokens_counts_nodes_pq.pop()
    current.value[0] = ""
    code_table = {}

    stack = []
    while current is not None or len(stack) > 0:
        while current is not None:
            prev_code = current.value[0]
            stack.append(current)
            current = current.left
            if current is not None:
                current.value[0] = prev_code + '0'
        current = stack.pop()
        if current.isleaf():
            code_table[current.value[1]] = current.value[0]
        prev_code = current.value[0]
        current = current.right
        if current is not None:
            current.value[0] = prev_code + '1'

    return current, code_table