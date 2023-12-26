#ifndef EXECUTOR_PROCESSOR_H
#define EXECUTOR_PROCESSOR_H

#include <stdbool.h>
#include <stdint.h>

#define RAM_SIZE (8 * 1024)
#define INT_REG_COUNT 32
#define FLOAT_REG_COUNT 32

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

void save_state(processor_t *proc, char *state_file_handle);
void load_state(processor_t *proc, char *state_file_handle);
binary load_executable(char *file_handle);
void run(processor_t *proc, binary *program);

#endif //EXECUTOR_PROCESSOR_H
