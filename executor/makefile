CFLAGS=-Wall -Wextra -Iinclude
FILES=./src/executor.c
build: $(FILES)
	gcc $(CFLAGS) $(FILES) -o ./build/executor

run: build
	./build/executor
