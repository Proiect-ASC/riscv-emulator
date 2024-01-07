#include "processor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void ex_1(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 192;
    assign_task(proc0, "../example_binaries/1.txt");
    memcpy(proc0->assigned_task.content + 24, str, strlen(str) + 1);
    save_state(proc0, "../state_files/1.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/1.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %d\n", proc0->assigned_task.program_end + 1, proc0->int_registers[22]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_2(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 192 + strlen(str) + 1;
    proc0->int_registers[23] = 192;
    assign_task(proc0, "../example_binaries/2.txt");
    memcpy(proc0->assigned_task.content + 24, str, strlen(str) + 1);
    save_state(proc0, "../state_files/2.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/2.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %s\n", proc0->assigned_task.program_end + 1, &proc0->ram[24 + strlen(str) + 1]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_3(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 392 + strlen(str) + 1;
    proc0->int_registers[23] = 392;
    proc0->int_registers[24] = 6;
    assign_task(proc0, "../example_binaries/3.txt");
    memcpy(proc0->assigned_task.content + 49, str, strlen(str) + 1);
    save_state(proc0, "../state_files/3.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/3.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %.*s\n", proc0->assigned_task.program_end + 1, proc0->int_registers[24], &proc0->ram[49 + strlen(str) + 1]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_4(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 648;
    assign_task(proc0, "../example_binaries/4.txt");
    memcpy(proc0->assigned_task.content + 81, str, strlen(str) + 1);
    save_state(proc0, "../state_files/4.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/4.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %s\n", proc0->assigned_task.program_end + 1, &proc0->ram[81]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_5(processor_t *proc0) {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 296;
    proc0->int_registers[23] = 11;
    assign_task(proc0, "../example_binaries/5.txt");
    memcpy(proc0->assigned_task.content + 37, a, 44);
    save_state(proc0, "../state_files/5.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/5.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %d\n", proc0->assigned_task.program_end + 1, proc0->int_registers[22]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_7(processor_t *proc0) {
    // because there is no 64 bit support, we must pod the values in memory
    float a[] = {0, 0, 0, 0};
    float b[] = {0, 1, 0, 1};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 256;
    proc0->int_registers[23] = 272;
    assign_task(proc0, "../example_binaries/7.txt");
    memcpy(proc0->assigned_task.content + 32, a, 16);
    memcpy(proc0->assigned_task.content + 48, b, 16);
    save_state(proc0, "../state_files/7.in");
    clock_t begin_clean = clock();
    run(proc0);
    clock_t end_clean = clock();
    double time_spent_clean = (double)(end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/7.out");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double)(end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %f\n", proc0->assigned_task.program_end + 1, proc0->float_registers[28]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean, time_spent_dirty - time_spent_clean, time_spent_dirty);
}

int main() {
    processor_t proc0;
    ex_7(&proc0);
    return 0;
}
