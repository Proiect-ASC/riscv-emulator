#ifndef EXECUTOR_PROCESSOR_H
#define EXECUTOR_PROCESSOR_H

#include <stdbool.h>
#include <stdint.h>
#include "treeloader.h"

#define RAM_SIZE (8 * 1024)
#define INT_REG_COUNT 32
#define FLOAT_REG_COUNT 32
#define ADDRESS_LENGHT 16

typedef struct binary {
    uint8_t remainder;
    uint16_t program_start, program_end;
    uint8_t content[RAM_SIZE];
} binary;

typedef struct processor_t {
    uint8_t ram[RAM_SIZE];
    uint16_t program_counter;
    int int_registers[INT_REG_COUNT];
    float float_registers[FLOAT_REG_COUNT];
} processor_t;

void save_state(const processor_t *proc, const char *state_file_handle);
void load_state(processor_t *proc, const char *state_file_handle);
binary load_executable(const char *file_handle);
extern inline uint8_t get_register(processor_t *proc, const huffman_tree *register_tree);
extern inline uint16_t get_address(processor_t *proc);
extern inline int get_int_immediate(processor_t *proc, uint8_t length);
extern inline void put_int_immediate(processor_t *proc, int imm, uint8_t length);
void run(processor_t *proc, const binary *program);

#endif //EXECUTOR_PROCESSOR_H
