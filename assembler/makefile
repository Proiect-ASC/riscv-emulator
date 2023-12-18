CFLAGS=-Wall -Wextra -Iinclude
FILES=./src/main.c ./src/parser/*.c ./src/encoder/*.c ./src/utils/*.c
HEADERS=./include/*

build: $(FILES)	$(HEADERS) 
	gcc $(CFLAGS) $(FILES) -o ./build/assembler

run: build
	./build/assembler
