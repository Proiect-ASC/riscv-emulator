import re
import pandas as pd
import scipy
import numpy as np


def read_tokens(file):
    with open(file, "r") as file:
        tokens = [token.strip() for token in file]
        return tokens


def count_tokens(file, tokens):
    with open(file, "r") as data:
        text = data.read()
        return {token: len(re.findall(token, text)) for token in tokens}


def compute_appearence_probabilities(tokens_counts):
    token_counts = pd.DataFrame(tokens_counts, index=[0])
    return np.asarray(token_counts.values).reshape(len(tokens_counts)) / np.asarray(token_counts.values).sum()


def compute_appearence_entropy(probs):
    return scipy.stats.entropy(probs, base=2)


tokens = read_tokens("instructions.txt")
tokens_counts = count_tokens("raw_data.txt", tokens)
tokens_probs = compute_appearence_probabilities(tokens_counts)
entropy = compute_appearence_entropy(tokens_probs)
print(entropy)
