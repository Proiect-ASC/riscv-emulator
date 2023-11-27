CFLAGS=-Wall -Wextra -Iinclude
FILES=./src/main.c ./src/parser/parser.c ./src/encoder/encoder.c
HEADERS=./include/parser.h ./include/encoder.h
OBJ_FILES=./build/main.o ./build/parser.o ./build/encoder.o ./build/treeloader.o
A=sample_input.txt

build: $(FILES)	$(HEADERS) 
	gcc -c $(CFLAGS) ./src/main.c -o ./build/main.o 
	gcc -c $(CFLAGS) ./src/parser/parser.c -o ./build/parser.o 
	gcc -c $(CFLAGS) ./src/encoder/encoder.c -o ./build/encoder.o 
	gcc -c $(CFLAGS) ./src/encoder/treeloader.c -o ./build/treeloader.o 
	gcc $(CFLAGS) $(OBJ_FILES) -o ./build/emulator

run: build
	./build/emulator $(A)
