import pandas as pd
from queue import PriorityQueue
from stats import read_tokens, count_tokens, compute_appearence_entropy, compute_appearence_probabilities

tokens = read_tokens("instructions.txt")
tokens_counts = count_tokens("raw_data.txt", tokens)
tokens_probs = compute_appearence_probabilities(tokens_counts)
entropy = compute_appearence_entropy(tokens_probs)

tokens_counts = [(v, k) for k, v in tokens_counts.items()]
tokens_counts_pq = PriorityQueue()

for item in tokens_counts:
    tokens_counts_pq.put(item)
