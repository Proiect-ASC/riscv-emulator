def serialize_code_table(code_table, output_file):
    with open(output_file, "w") as output_file:
        for k, v in code_table.items():
            output_file.write(f"{k} {v}\n")
