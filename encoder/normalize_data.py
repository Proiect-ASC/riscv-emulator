# i noticed some inconsistent whitespace in the raw data that makes it hard to encode instructions with their operands
# this "normalizes" all whitespace to be monospaced

def normalize_data(raw_data):
    with open(raw_data, "r+") as data:
        text = data.readlines()
        data.seek(0)
        data.truncate()
        for line in text:
            tokens = line.split()
            line = ' '.join(tokens)
            data.write(line + '\n')


normalize_data("raw_data.txt")