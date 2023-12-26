#include "processor.h"
#include "treeloader.h"
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
    huffman_tree instruction_tree = load_huffman_tree("../huffman_trees/instructions_huffman_tree.txt", false),
        register_tree = load_huffman_tree("../huffman_trees/registers_huffman_tree.txt", true);
    void *instruction_labels[] = {
            &&li,
            &&add,
            &&lb,
            &&beqz,
            &&addi,
            &&j,
            &&mv,
            &&ret,
            &&sb,
            &&bge,
            &&sd,
            &&srai,
            &&sub,
            &&ld,
            &&slli,
            &&lw,
            &&ble,
            &&bnez,
            &&fld,
            &&fsw,
            &&la,
            &&bgt,
            &&flw,
            &&fadd_s,
            &&fmul_s,
            &&fmv_s,
            &&fmv_s_x,
            &&flt_s,
            &&fgt_s,
            &&fsqrt_d,
            &&fadd_d,
            &&fmul_d,
            &&fsub_d,
            &&call_printf,
            &&call_scanf,
            &&call_strlen,
            &&call_cfunc,
            &&call_intern
    };
    proc->program_counter = program->program_start;
    uint16_t program_end = program->program_end;
    memcpy(proc->ram, program->content, RAM_SIZE);
    uint8_t index = 0;
    next_instr:
        while (instruction_tree.nodes[index].value == -1) {
            int curr_byte = proc->program_counter / 8;
            if (proc->ram[curr_byte] & (1 << (proc->program_counter++ % 8))) {
                index = instruction_tree.nodes[index].right_index;
            }
            else {
                index = instruction_tree.nodes[index].left_index;
            }
        }
        int jump_index = instruction_tree.nodes[index].value;
        index = 0;
        goto *instruction_labels[jump_index];
    li:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    add:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    lb:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    beqz:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    addi:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    j:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    mv:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ret:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sb:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bge:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sd:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    srai:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sub:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ld:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    slli:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    lw:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ble:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bnez:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fld:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsw:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    la:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bgt:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    flw:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fadd_s:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmul_s:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmv_s:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmv_s_x:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    flt_s:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fgt_s:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsqrt_d:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fadd_d:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmul_d:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsub_d:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    call_printf:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    call_scanf:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    call_strlen:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    call_cfunc:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    call_intern:
        // TODO: implement
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    end:
        // TODO: implement
}
