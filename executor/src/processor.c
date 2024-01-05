#include "processor.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void save_state(const processor_t *proc, const char *state_file_handle) {
    FILE *state_file = fopen(state_file_handle, "w");
    fwrite(proc->ram, 1, RAM_SIZE, state_file);
    fwrite(&proc->program_counter, 2, 1, state_file);
    fwrite(proc->int_registers, 4, 32, state_file);
    fwrite(proc->float_registers, 4, 32, state_file);
    fclose(state_file);
}

void load_state(processor_t *proc, const char *state_file_handle) {
    FILE *state_file = fopen(state_file_handle, "r");
    fread(proc->ram, 1, RAM_SIZE, state_file);
    fread(&proc->program_counter, 2, 1, state_file);
    fread(proc->int_registers, 4, 32, state_file);
    fread(proc->float_registers, 4, 32, state_file);
    fclose(state_file);
}

void assign_task(processor_t *proc, const char *file_handle) {
    FILE *binary_file = fopen(file_handle, "r");
    fread(&proc->assigned_task.remainder, 1, 1, binary_file);
    fread(&proc->assigned_task.program_start, 2, 1, binary_file);
    fread(&proc->assigned_task.content, 1, RAM_SIZE, binary_file);
    fseek(binary_file, 0, SEEK_END);
    proc->assigned_task.program_end = 8 * ftell(binary_file) - proc->assigned_task.remainder - 24 - 1;
    fclose(binary_file);
}

static inline uint16_t get_address(processor_t *proc) {
    uint16_t addr = 0;
    for (int i = 0; i < ADDRESS_LENGTH; ++i) {
        uint16_t curr_byte = proc->program_counter / 8;
        if (proc->ram[curr_byte] & (1 << (7 - proc->program_counter++ % 8))) {
            addr += (1 << (7 - i % 8 + (i / 8) * 8));
        }
    }
    return addr;
}

static inline uint8_t get_register(processor_t *proc, const huffman_tree *register_tree) {
    uint8_t index = 0;
    while (register_tree->nodes[index].value == -1) {
        uint16_t curr_byte = proc->program_counter / 8;
        if (proc->ram[curr_byte] & (1 << (7 - proc->program_counter++ % 8))) {
            index = register_tree->nodes[index].right_index;
        }
        else {
            index = register_tree->nodes[index].left_index;
        }
    }
    return register_tree->nodes[index].value;
}

static inline void compute_heap_addr(processor_t *proc) {
    if(proc->program_counter > (proc->assigned_task.program_end + proc->assigned_task.remainder)) {
        int addr_base = proc->assigned_task.program_end + proc->assigned_task.remainder + 1;
        int byte_offset = proc->program_counter - addr_base;
        proc->program_counter = addr_base + byte_offset * 8;
    }
}

static inline int get_int_immediate(processor_t *proc, uint8_t length) {
    int val = 0;
    if(proc->program_counter >= proc->assigned_task.program_end && proc->program_counter <= proc->assigned_task.program_end + proc->assigned_task.remainder) {
        fprintf(stderr, "[ERROR] Cannot get integer immediate. Access violation at address 0x%X, terminating program...\n", proc->program_counter);
        exit(1);
    }
    compute_heap_addr(proc);
    for (int i = 0; i < length; ++i) {
        uint16_t curr_byte = proc->program_counter / 8;
        if (proc->ram[curr_byte] & (1 << (7 - proc->program_counter++ % 8))) {
            val += (1 << (7 - i % 8 + (i / 8) * 8));
        }
    }
    return val;
}

static inline void put_int_immediate(processor_t *proc, int imm, uint8_t length) {
    if(proc->program_counter >= proc->assigned_task.program_start && proc->program_counter <= proc->assigned_task.program_end + proc->assigned_task.remainder) {
        fprintf(stderr, "[ERROR] Cannot put integer immediate. Access violation at address 0x%X, terminating program...\n", proc->program_counter);
        exit(1);
    }
    compute_heap_addr(proc);
    for (int i = 0; i < length; ++i) {
        uint16_t curr_byte = proc->program_counter / 8;
        proc->ram[curr_byte] &= ((uint8_t)0xFF ^ (1 << (7 - proc->program_counter % 8)));
        if (imm & (1 << (7 - i % 8 + (i / 8) * 8))) {
            proc->ram[curr_byte] |= (1 << (7 - proc->program_counter % 8));
        }
        proc->program_counter++;
    }
}

static inline float get_float_immediate(processor_t *proc) {
    if(proc->program_counter >= proc->assigned_task.program_end && proc->program_counter <= proc->assigned_task.program_end + proc->assigned_task.remainder) {
        fprintf(stderr, "[ERROR] Cannot get float immediate. Access violation at address 0x%X, terminating program...\n", proc->program_counter);
        exit(1);
    }
    compute_heap_addr(proc);
    intfloat val;
    val.i = 0;
    for (int i = 0; i < 32; ++i) {
        uint16_t curr_byte = proc->program_counter / 8;
        if (proc->ram[curr_byte] & (1 << (7 - proc->program_counter++ % 8))) {
            val.i += (1 << (7 - i % 8 + (i / 8) * 8));
        }
    }
    return val.f;
}

static inline void put_float_immediate(processor_t *proc, float imm) {
    if(proc->program_counter >= proc->assigned_task.program_start && proc->program_counter <= proc->assigned_task.program_end + proc->assigned_task.remainder) {
        fprintf(stderr, "[ERROR] Cannot put float immediate. Access violation at address 0x%X, terminating program...\n", proc->program_counter);
        exit(1);
    }
    compute_heap_addr(proc);
    intfloat val;
    val.f = imm;
    for (int i = 0; i < 32; ++i) {
        uint16_t curr_byte = proc->program_counter / 8;
        proc->ram[curr_byte] &= ((uint8_t)0xFF ^ (1 << (7 - proc->program_counter % 8)));
        if (val.i & (1 << (7 - i % 8 + (i / 8) * 8))) {
            proc->ram[curr_byte] |= (1 << (7 - proc->program_counter % 8));
        }
        proc->program_counter++;
    }
}

void run(processor_t *proc) {
    /* to match instructions or registers to indices, look up the huffman tree and match, in order, each item with the
     * respective position in the array, n-th item to n-th array position, such as
     * a4 ... register_indices[4] = 4 (because a4 is the 5th int register to appear)
     * or
     * fa3 ... register_indices[20] = 0 (because fa3 is the first float register to appear but the 21th overall) */
    int instruction_indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                                 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                 19, 20, 21, 22, 23, 24, 25, 26, 27,
                                 28, 29, 30, 31, 32, 33, 34, 35, 36,
                                 37};
    int register_indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                              10, 11, 12, 13, 14, 15, 16, 17, 18,
                              19, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                              10, 11, 12, 13, 14, 15, 16, 17, 18,
                              19, 20, 21, 22, 23, 24, 20, 21, 25, 26,
                              22, 23, 27, 28, 24, 25, 29, 30, 26, 27,
                              28, 29, 31, 30, 31};
    huffman_tree instruction_tree = load_huffman_tree("../huffman_trees/instructions_huffman_tree.txt", instruction_indices),
        register_tree = load_huffman_tree("../huffman_trees/registers_huffman_tree.txt", register_indices);
    void *instruction_labels[] = {
            &&addi,
            &&j,
            &&slli,
            &&lw,
            &&ble,
            &&bnez,
            &&call_cfunc,
            &&call_scanf,
            &&call_strlen,
            &&fadd_d,
            &&fmul_d,
            &&fgt_s,
            &&fsqrt_d,
            &&mv,
            &&ret,
            &&li,
            &&add,
            &&call_intern,
            &&flw,
            &&fadd_s,
            &&fmv_s_x,
            &&flt_s,
            &&sub,
            &&ld,
            &&lb,
            &&beqz,
            &&sb,
            &&bge,
            &&fld,
            &&fsw,
            &&fmul_s,
            &&fmv_s,
            &&fsub_d,
            &&call_printf,
            &&la,
            &&bgt,
            &&sd,
            &&srai
    };
    proc->program_counter = proc->assigned_task.program_start;
    uint16_t program_end = proc->assigned_task.program_end;
    proc->int_registers[18] = 0; // zero is zero B)
    proc->int_registers[19] = program_end + 1; // return address (ra) to program end + 1 from the test examples, because no main is defined
    proc->int_registers[28] = RAM_SIZE + 7 * (proc->assigned_task.program_end + proc->assigned_task.remainder + 1) / 8 - 1 ; // stack pointer is at bottom of the stack
    memcpy(proc->ram, proc->assigned_task.content, RAM_SIZE);
    uint8_t index = 0;
    next_instr:
        while (instruction_tree.nodes[index].value == -1) {
            uint16_t curr_byte = proc->program_counter / 8;
            if (proc->ram[curr_byte] & (1 << (7 - proc->program_counter++ % 8))) {
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
        ;
        int rd_li = get_register(proc, &register_tree);
        int imm_li = get_int_immediate(proc, 32);
        proc->int_registers[rd_li] = imm_li;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    add:
        ;
        int rd_add = get_register(proc, &register_tree);
        int rs1_add = get_register(proc, &register_tree);
        int rs2_add = get_register(proc, &register_tree);
        proc->int_registers[rd_add] = proc->int_registers[rs1_add] + proc->int_registers[rs2_add];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    lb:
        ;
        int rd_lb = get_register(proc, &register_tree);
        int dest_addr_lb = get_address(proc);
        int dest_addr_reg_lb = get_register(proc, &register_tree);
        int return_addr_lb = proc->program_counter;
        proc->program_counter = dest_addr_lb + proc->int_registers[dest_addr_reg_lb];
        int imm_lb = get_int_immediate(proc, 8);
        proc->program_counter = return_addr_lb;
        //nifty cast trick to sign extend an 8 bit imm
        imm_lb = (int8_t)imm_lb;
        imm_lb = (int)imm_lb;
        proc->int_registers[rd_lb] = imm_lb;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    beqz:
        ;
        int rd_beqz = get_register(proc, &register_tree);
        int dest_addr_beqz = get_address(proc);
        if (proc->int_registers[rd_beqz] == 0) proc->program_counter = dest_addr_beqz;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    addi:
        ;
        int rd_addi = get_register(proc, &register_tree);
        int rs1_addi = get_register(proc, &register_tree);
        int imm_addi = get_int_immediate(proc, 32);
        proc->int_registers[rd_addi] = proc->int_registers[rs1_addi] + imm_addi;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    j:
        ;
        int dest_addr_j = get_address(proc);
        proc->program_counter = dest_addr_j;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    mv:
        ;
        int rd_mv = get_register(proc, &register_tree);
        int rs1_mv = get_register(proc, &register_tree);
        proc->int_registers[rd_mv] = proc->int_registers[rs1_mv];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ret:
        proc->program_counter = proc->int_registers[19];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sb:
        ;
        int rs_sb = get_register(proc, &register_tree);
        int dest_addr_sb = get_address(proc);
        int dest_addr_reg_sb = get_register(proc, &register_tree);
        int return_addr_sb = proc->program_counter;
        proc->program_counter = dest_addr_sb + proc->int_registers[dest_addr_reg_sb];
        put_int_immediate(proc, proc->int_registers[rs_sb], 8);
        proc->program_counter = return_addr_sb;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bge:
        ; 
        int r1_bge = get_register(proc, &register_tree);
        int r2_bge = get_register(proc, &register_tree);
        int dest_addr_bge = get_address(proc);
        if (proc->int_registers[r1_bge] >= proc->int_registers[r2_bge]) proc->program_counter = dest_addr_bge;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sd:
        ;
        int rd_sd = get_register(proc, &register_tree);
        int dest_addr_sd = get_address(proc);
        int dest_addr_reg_sd = get_register(proc, &register_tree);
        int return_addr_sd = proc->program_counter;
        proc->program_counter = dest_addr_sd + proc->int_registers[dest_addr_reg_sd];
        put_int_immediate(proc, proc->int_registers[rd_sd], 32);
        proc->program_counter = return_addr_sd;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    srai:
        ;
        int rd_srai = get_register(proc, &register_tree);
        int rs_srai = get_register(proc, &register_tree);
        int imm_srai = get_int_immediate(proc, 32);
        signedunsigned bitmask;
        bitmask.u = ~(~0U >> imm_srai);
        bool is_rs_negative_srai = proc->int_registers[rs_srai] < 0;
        proc->int_registers[rd_srai] = (proc->int_registers[rs_srai] >> imm_srai);
        if (is_rs_negative_srai) proc->int_registers[rd_srai] |= bitmask.s;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    sub:
        ;
        int rd_sub = get_register(proc, &register_tree);
        int rs1_sub = get_register(proc, &register_tree);
        int rs2_sub = get_register(proc, &register_tree);
        proc->int_registers[rd_sub] = proc->int_registers[rs1_sub] - proc->int_registers[rs2_sub];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ld:
        ;
        int rd_ld = get_register(proc, &register_tree);
        int dest_addr_ld = get_address(proc);
        int dest_addr_reg_ld = get_register(proc, &register_tree);
        int return_addr_ld = proc->program_counter;
        proc->program_counter = dest_addr_ld + proc->int_registers[dest_addr_reg_ld];
        int imm_ld = get_int_immediate(proc, 32);
        proc->program_counter = return_addr_ld;
        proc->int_registers[rd_ld] = imm_ld;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    slli:
        ;
        int rd_slli = get_register(proc, &register_tree);
        int rs_slli = get_register(proc, &register_tree);
        int imm_slli = get_int_immediate(proc, 32);
        proc->int_registers[rd_slli] = proc->int_registers[rs_slli] << imm_slli;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    lw:
        ;
        int rd_lw = get_register(proc, &register_tree);
        int dest_addr_lw = get_address(proc);
        int dest_addr_reg_lw = get_register(proc, &register_tree);
        int return_addr_lw = proc->program_counter;
        proc->program_counter = dest_addr_lw + proc->int_registers[dest_addr_reg_lw];
        int imm_lw = get_int_immediate(proc, 32);
        proc->program_counter = return_addr_lw;
        proc->int_registers[rd_lw] = imm_lw;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    ble:
        ; 
        int r1_ble = get_register(proc, &register_tree);
        int r2_ble = get_register(proc, &register_tree);
        int dest_addr_ble = get_address(proc);
        if (proc->int_registers[r1_ble] <= proc->int_registers[r2_ble]) proc->program_counter = dest_addr_ble;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bnez:
        ;
        int rd_bnez = get_register(proc, &register_tree);
        int dest_addr_bnez = get_address(proc);
        if (proc->int_registers[rd_bnez] != 0) proc->program_counter = dest_addr_bnez;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fld:
        ;
        int rd_fld = get_register(proc, &register_tree);
        uint16_t dest_addr_fld = get_address(proc);
        int dest_addr_reg_fld = get_register(proc, &register_tree);
        int return_addr_fld = proc->program_counter;
        proc->program_counter = dest_addr_fld + proc->int_registers[dest_addr_reg_fld];
        float imm_fld = get_float_immediate(proc);
        proc->program_counter = return_addr_fld;
        proc->float_registers[rd_fld] = imm_fld;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsw:
        ;
        int rs_fsw = get_register(proc, &register_tree);
        int dest_addr_fsw = get_address(proc);
        uint16_t dest_addr_reg_fsw = get_register(proc, &register_tree);
        int return_addr_fsw = proc->program_counter;
        proc->program_counter = dest_addr_fsw + proc->int_registers[dest_addr_reg_fsw];
        put_float_immediate(proc, proc->float_registers[rs_fsw]);
        proc->program_counter = return_addr_fsw;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    la:
        ;
        int rd_la = get_register(proc, &register_tree);
        int source_load_address = get_address(proc);
        proc->int_registers[rd_la] = source_load_address;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    bgt:
        ;
        int r1_bgt = get_register(proc, &register_tree);
        int r2_bgt = get_register(proc, &register_tree);
        int dest_addr_bgt = get_address(proc);
        if (proc->int_registers[r1_bgt] > proc->int_registers[r2_bgt]) proc->program_counter = dest_addr_bgt;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    flw:
        ;
        int rd_flw = get_register(proc, &register_tree);
        uint16_t dest_addr_flw = get_address(proc);
        int dest_addr_reg_flw = get_register(proc, &register_tree);
        int return_addr_flw = proc->program_counter;
        proc->program_counter = dest_addr_flw + proc->int_registers[dest_addr_reg_flw];
        float imm_flw = get_float_immediate(proc);
        proc->program_counter = return_addr_flw;
        proc->float_registers[rd_flw] = imm_flw;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fadd_s:
        ;
        int rd_fadd_s = get_register(proc, &register_tree);
        int rs1_fadd_s = get_register(proc, &register_tree);
        int rs2_fadd_s = get_register(proc, &register_tree);
        proc->float_registers[rd_fadd_s] = proc->float_registers[rs1_fadd_s] + proc->float_registers[rs2_fadd_s];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmul_s:
        ;
        int rd_fmul_s = get_register(proc, &register_tree);
        int rs1_fmul_s = get_register(proc, &register_tree);
        int rs2_fmul_s = get_register(proc, &register_tree);
        proc->float_registers[rd_fmul_s] = proc->float_registers[rs1_fmul_s] * proc->float_registers[rs2_fmul_s];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmv_s:
        ;
        int rd_fmv_s = get_register(proc, &register_tree);
        int rs_fmv_s = get_register(proc, &register_tree);
        proc->float_registers[rd_fmv_s] = proc->float_registers[rs_fmv_s];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmv_s_x:
        ;
        int rd_fmv_s_x = get_register(proc, &register_tree);
        int rs_fmv_s_x = get_register(proc, &register_tree);
        intfloat temp_fmv_s_x;
        temp_fmv_s_x.f = proc->float_registers[rs_fmv_s_x];
        proc->int_registers[rd_fmv_s_x] = temp_fmv_s_x.i;
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    flt_s:
        ;
        int rd_flt_s = get_register(proc, &register_tree);
        int rs1_flt_s = get_register(proc, &register_tree);
        int rs2_flt_s = get_register(proc, &register_tree);
        if (proc->float_registers[rs1_flt_s] < proc->float_registers[rs2_flt_s]) {
            proc->float_registers[rd_flt_s] = 1;
        } else {
            proc->float_registers[rd_flt_s] = 0;
        }
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fgt_s:
        ;
        int rd_fgt_s = get_register(proc, &register_tree);
        int rs1_fgt_s = get_register(proc, &register_tree);
        int rs2_fgt_s = get_register(proc, &register_tree);
        if (proc->float_registers[rs1_fgt_s] > proc->float_registers[rs2_fgt_s]) {
            proc->float_registers[rd_fgt_s] = 1;
        } else {
            proc->float_registers[rd_fgt_s] = 0;
        }
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsqrt_d:
        ;
        int rd_fsqrt_d = get_register(proc, &register_tree);
        int rs1_fsqrt_d = get_register(proc, &register_tree);
        proc->float_registers[rd_fsqrt_d] = sqrt(proc->float_registers[rs1_fsqrt_d]);
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fadd_d:
        ;
        int rd_fadd_d = get_register(proc, &register_tree);
        int rs1_fadd_d = get_register(proc, &register_tree);
        int rs2_fadd_d = get_register(proc, &register_tree);
        proc->float_registers[rd_fadd_d] = proc->float_registers[rs1_fadd_d] + proc->float_registers[rs2_fadd_d];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fmul_d:
        ;
        int rd_fmul_d = get_register(proc, &register_tree);
        int rs1_fmul_d = get_register(proc, &register_tree);
        int rs2_fmul_d = get_register(proc, &register_tree);
        proc->float_registers[rd_fmul_d] = proc->float_registers[rs1_fmul_d] * proc->float_registers[rs2_fmul_d];
        if (proc->program_counter > program_end) {
            goto end;
        }
        goto next_instr;
    fsub_d:
        ;
        int rd_fsub_d = get_register(proc, &register_tree);
        int rs1_fsub_d = get_register(proc, &register_tree);
        int rs2_fsub_d = get_register(proc, &register_tree);
        proc->float_registers[rd_fsub_d] = proc->float_registers[rs1_fsub_d] - proc->float_registers[rs2_fsub_d];
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
        ;
        int return_addr_call_strlen = proc->program_counter;
        proc->program_counter = proc->int_registers[22];
        compute_heap_addr(proc);
        proc->int_registers[22] = strlen(&proc->ram[proc->program_counter / 8]);
        proc->program_counter = return_addr_call_strlen;
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
