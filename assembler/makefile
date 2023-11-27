CFLAGS=-Wall -Wextra -Iinclude
FILES=./src/main.c ./src/parser/parser.c ./src/encoder/encoder.c ./src/encoder/treeloader.c ./src/utils/utils.c
HEADERS=./include/*
A=sample_input.txt

build: $(FILES)	$(HEADERS) 
	gcc $(CFLAGS) $(FILES) -o ./build/assembler

run: build
	./build/assembler $(A)
