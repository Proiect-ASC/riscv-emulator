#ifndef EXECUTOR_PROCESSOR_H
#define EXECUTOR_PROCESSOR_H

#include "treeloader.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define RAM_SIZE (8 * 1024)
#define INT_REG_COUNT 32
#define FLOAT_REG_COUNT 32
#define ADDRESS_LENGTH 16

typedef struct binary {
    uint8_t remainder;
    uint16_t program_start, program_end;
    uint8_t content[RAM_SIZE];
} binary;

typedef struct processor_t {
    uint8_t ram[RAM_SIZE];
    uint16_t memory_indicator; // acts like PC when viewed from inside the VM
    int int_registers[INT_REG_COUNT];
    float float_registers[FLOAT_REG_COUNT];
    binary assigned_task;
} processor_t;

typedef union intfloat {
    int32_t i;
    float f;
} intfloat;

typedef union signedunsigned {
    int32_t s;
    uint32_t u;
} signedunsigned;

typedef union printf_arg {
    char *s;
    int d;
} printf_arg;

void save_state(const processor_t *proc, const char *state_file_handle);

void load_state(processor_t *proc, const char *state_file_handle);

void assign_task(processor_t *proc, const char *file_handle);

static inline uint16_t get_address(processor_t *proc);

static inline rgstr get_register(processor_t *proc, const huffman_tree *register_tree);

static inline void compute_heap_addr(processor_t *proc);

static inline int get_int_immediate(processor_t *proc, uint8_t length);

static inline void put_int_immediate(processor_t *proc, int imm, uint8_t length);

static inline float get_float_immediate(processor_t *proc);

static inline void put_float_immediate(processor_t *proc, float imm);

void run(processor_t *proc, bool must_load_into_ram, bool load_and_quit);

#endif //EXECUTOR_PROCESSOR_H
