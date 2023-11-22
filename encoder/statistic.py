import re
import pandas as pd
import scipy
import numpy as np

def count_tokens(file, tokens):
    with open(file, "r") as data:
        text = data.read()
        return {token: len(re.findall(token, text)) for token in tokens}


tokens = ["li", "add", "lb", "beqz", "addi", "j", "mv", "ret", "sb", "bge", "sd", "mv", "call", "srai", "sub", "ld", "slli", "lw", "ble", "bnez", "fld", "fsw", "la", "bgt", "flw", "fadd.s", "fmul.s", "fmul.s.x", "fmv.s", "flt.s", "fgt.s", "fsqrt.d", "fadd.d", "fmul.d", "fsub.d"]
token_counts = pd.DataFrame(count_tokens("raw_data.txt", tokens), index=[0])
pk = np.asarray(token_counts.values).reshape(34) / np.asarray(token_counts.values).sum()
entropy = scipy.stats.entropy(pk, base=2)
print(entropy)