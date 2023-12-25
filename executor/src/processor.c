#include "processor.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void save_state(processor_t *proc, char *state_file_handle) {
    FILE *state_file = fopen(state_file_handle, "w");
    fwrite(proc->ram, 1, RAM_SIZE, state_file);
    fwrite(&proc->program_counter, 2, 1, state_file);
    fwrite(proc->int_registers, 4, 32, state_file);
    fwrite(proc->float_registers, 4, 32, state_file);
}

void load_state(processor_t *proc, char *state_file_handle) {
    FILE *state_file = fopen(state_file_handle, "r");
    fread(proc->ram, 1, RAM_SIZE, state_file);
    fread(&proc->program_counter, 2, 1, state_file);
    fread(proc->int_registers, 4, 32, state_file);
    fread(proc->float_registers, 4, 32, state_file);
}

binary load_executable(char *file_handle) {
    FILE *binary_file = fopen(file_handle, "r");
    binary program;
    fread(&program.remainder, 1, 1, binary_file);
    fread(&program.program_start, 2, 1, binary_file);
    fread(&program.content, 1, RAM_SIZE, binary_file);
    fseek(binary_file, 0, SEEK_END);
    program.program_end = 8 * ftell(binary_file) - program.remainder - 24;
    return program;
}

void run(processor_t *proc, binary *program) {
    proc->program_counter = program->program_start;
    uint16_t program_end = program->program_end;
    memcpy(proc->ram, program->content, RAM_SIZE);
    // TODO: read huffman trees and modify them to have pointers to the registers
}