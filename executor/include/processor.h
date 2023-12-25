#ifndef EXECUTOR_PROCESSOR_H
#define EXECUTOR_PROCESSOR_H

#include <stdbool.h>
#include <stdint.h>

#define RAM_SIZE (8 * 1024)
#define INT_REG_COUNT 32
#define FLOAT_REG_COUNT 32

typedef enum int_registers {
    ZERO,
    RA,
    SP,
    GP,
    TP,
    T0,
    T1,
    T2,
    S0,
    S1,
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    S8,
    S9,
    S10,
    S11,
    T3,
    T4,
    T5,
    T6
} int_registers;

typedef enum float_registers {
    FT0,
    FT1,
    FT2,
    FT3,
    FT4,
    FT5,
    FT6,
    FT7,
    FS0,
    FS1,
    FA0,
    FA1,
    FA2,
    FA3,
    FA4,
    FA5,
    FA6,
    FA7,
    FS2,
    FS3,
    FS4,
    FS5,
    FS6,
    FS7,
    FS8,
    FS9,
    FS10,
    FS11,
    FT8,
    FT9,
    FT10,
    FT11
} float_registers;

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
