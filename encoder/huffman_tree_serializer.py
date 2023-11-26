def serialize(huffman_tree, output_file):
    with open(output_file, "w") as output_file:
        current = huffman_tree
        stack = []
        while current is not None or len(stack) > 0:
            while current is not None:
                stack.append(current)
                current = current.left
                if current is not None:
                    output = current.value[0]
                    if current.isleaf():
                        output += ' ' + current.value[1]
                    output_file.write(output + '\n')

            current = stack.pop()

            current = current.right
            if current is not None:
                output = current.value[0]
                if current.isleaf():
                    output += ' ' + current.value[1]
                output_file.write(output + '\n')
