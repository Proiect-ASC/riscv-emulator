import re
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
    appearences = list(tokens_counts.values())
    return np.asarray(appearences).reshape(len(tokens_counts)) / np.asarray(appearences).sum()


def compute_appearence_entropy(probs):
    return scipy.stats.entropy(probs, base=2)
