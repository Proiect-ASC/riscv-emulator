CFLAGS=-Wall -Wextra -Iinclude
FILES=./src/main.c ./src/parser/parser.c
A=sample_input.txt

build: $(FILES)
	gcc $(CFLAGS) $(FILES) -o ./build/emulator

run: build
	./build/emulator $(A)
