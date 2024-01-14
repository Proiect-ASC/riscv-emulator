# Intro

This is our project for the 2023 computer architecture class.
It consists of an assembler and an executor for RISC-V assembly language.

# The contributors (and their respective contributions)

The following contributors are from group 152 (except Izabela, she is from group 151), 1st year:

- Tiberiu Popescu: has created the assembler for this project

- Voicu Ioan - Vladut: has designed the executor

- Jilavu Izabela: has implemented float RISC-V instructions for the executor

- Maracine Razvan: has implemented conditional jumps and aritmetic RISC-V instructions for the executor

# Project description

### Assembler

###### Assembler description:
The assembler was fun to write and it came with its unique challenges. We chose to employ a Definite Finite Automaton
(DFA) for the lexer part of it, because it is a robust implementation and does not leave room for errors that could
arrise from naive string operations. After the input file is tokenized by the lexer, we chose to write a simple
parser that validates the input and makes sure that every instruction has appropriate parameters. This step was
not necessary because all the inputs are assumed to be correct, but we thought it'd be a nice touch to have our project
work in a more general use case. The making of the parser was pretty straight forward, because the grammar of assembly
languages is simple and does not require complex structures like an Abstract Syntax Tree. The last part of the assembler
is the encoder, the core of the program, that translates tokens to binary and groups bits 8 by 8 to be then written
as bytes in the output file.

###### Assembler usage:
The assembler must be run from the same file code_table.txt is located in, or it won't work;
To assemble a file, simply run this command:
```
./<path_to_assembler> <input_file> <output_file>
```
Example usage, if assembler is located in ./build/assembler and code_table.txt in ./code_table.txt, and we want to
assemble the file ./example_source.txt:
```
./build/assembler examble_source.txt output_binary
```

### Executor

###### Executor description:
The executor took inspiration from Forth machines, featuring a Huffman threading approach to dispatching
instructions. It also implements a modular architecture, so changes to the register count or types is easily done.
One mention is that by the professor's request, we had to limit ourselves to 32-bit immediates, but that is easily
changeable (we use 64 bits to store/load during execution, but only use the lower 32 bits). Designing the addressing
scheme was particulary interesting, due to the binary compression we used, the code segment
needs bit addressable memory, but the example programs use byte addressable memory for computing array offsets and
stack pointer operations. As such, we feature a hybrid memory scheme, where only the code segment is bit addressable,
the heap and stack being byte addressable (and byte aligned). The addresses depend on the binary size, so no two
programs with different sizes will have similar address spaces. Extern function calls are implemented via an extern
function call dispatcher, which uses separate .c and .h files to store function implementations, so only those have to
be rebuilt in order to update the available extern functions (because the main file is dynamically linked to the extern
function caller, it does not require the whole project recompilation). This is a tehnique Dave Cutler (the mind behind Windows)
also used.

###### Executor usage:

Attention! Generating statefiles uses relative paths. Make sure to run the project inside a folder!

Use the following command to display usage:

```
./executor -h
```

To run the examples you must run the binaries with their respective statefile, using the
```-s``` flag. Check the CLI help for further instructions. Warning: ```-g``` flag rewrites the statefiles, and supposes that 
the loaded binary can run without an input statefile.
