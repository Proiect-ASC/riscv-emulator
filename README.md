# Intro

This is our project for the 2023 computer architecture class.
It consists of an assembler and an executor for RISC-V assembly language.

# The contributors (and their respective contributions)

The following contributors are from group 152, 1st year:

- Tiberiu Popescu: has created the assembler for this project

- Voicu Ioan - Vladut: has designed the executor

- Jilavu Izabela: has implemented float RISC-V instructions for the executor

- Maracine Razvan: has implemented conditional jumps and aritmetic RISC-V instructions for the executor

# Project description

###``` Executor

###### Executor description:
The executor took inspiration from Forth machines, featuring a Huffman threading approach to dispatching
instructions. It also implements a modular architecture, so changes to the register count or types is easily done.
One mention is that by the professor's request, we had to limit ourselves to 32-bit immediates, but that is easily
changeable (we use 64 bits to store/load during execution, but only use the lower 32 bits). Designing the addressing
scheme was particulary interesting, due to the binary compression we used, the code segment
needs bit addressable memory, but the exeample programs use byte addressable memory for computing array offsets and
stack pointer operations. As such, we feature a hybrid memory scheme, where only the code segment is bit addressable,
the heap and stack being byte addressable (and byte aligned). The addresses depend on the binary size, so no two
programs with different sizes will have similar address spaces. Extern function calls are implemented via an extern
function call dispatcher, which uses separate .c and .h files to store function implementations, so only those have to
be
rebuilt in order to update the available extern functions (because the main file is dynamically linked to the extern
function
caller, it does not require the whole project recompilation). This is a tehnique Dave Cutler (the mind behind Windows)
also
used.

###### Executor usage:

Attention! Generating statefiles uses relative paths. Make sure to run the project inside a folder!

Use the following command to display usage:

```
./executor -h
```