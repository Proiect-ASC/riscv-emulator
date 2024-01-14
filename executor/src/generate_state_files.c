#include "generate_state_files.h"

void ex_1(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 192;
    assign_task(proc0, "../example_binaries/1.txt");
    memcpy(proc0->assigned_task.content + 24, str, strlen(str) + 1);
    run(proc0, true, true);
    save_state(proc0, "../state_files/1in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/1out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %d\n", proc0->assigned_task.program_end + 1,
           proc0->int_registers[22]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_2(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = (int) (192 + strlen(str) + 1);
    proc0->int_registers[23] = 192;
    assign_task(proc0, "../example_binaries/2.txt");
    memcpy(proc0->assigned_task.content + 24, str, strlen(str) + 1);
    run(proc0, true, true);
    save_state(proc0, "../state_files/2in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/2out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %s\n", proc0->assigned_task.program_end + 1,
           &proc0->ram[24 + strlen(str) + 1]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_3(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = (int) (392 + strlen(str) + 1);
    proc0->int_registers[23] = 392;
    proc0->int_registers[24] = 6;
    assign_task(proc0, "../example_binaries/3.txt");
    memcpy(proc0->assigned_task.content + 49, str, strlen(str) + 1);
    run(proc0, true, true);
    save_state(proc0, "../state_files/3in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/3out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %.*s\n", proc0->assigned_task.program_end + 1,
           proc0->int_registers[24], &proc0->ram[49 + strlen(str) + 1]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_4(processor_t *proc0) {
    char *str = "ana are mere";
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 648;
    assign_task(proc0, "../example_binaries/4.txt");
    memcpy(proc0->assigned_task.content + 81, str, strlen(str) + 1);
    run(proc0, true, true);
    save_state(proc0, "../state_files/4in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/4out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %s\n", proc0->assigned_task.program_end + 1,
           &proc0->ram[81]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_5(processor_t *proc0) {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 296;
    proc0->int_registers[23] = 11;
    assign_task(proc0, "../example_binaries/5.txt");
    memcpy(proc0->assigned_task.content + 37, a, 44);
    run(proc0, true, true);
    save_state(proc0, "../state_files/5in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/5out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %d\n", proc0->assigned_task.program_end + 1,
           proc0->int_registers[22]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_6(processor_t *proc0) {
    // because there is no 64 bit support, we must pod the values in memory
    int a[] = {0, 3, 0, 1, 0, 4, 0, 10};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 464;
    proc0->int_registers[23] = 4;
    assign_task(proc0, "../example_binaries/6.txt");
    memcpy(proc0->assigned_task.content + 59, a, 32);
    run(proc0, true, true);
    save_state(proc0, "../state_files/6in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/6out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: ", proc0->assigned_task.program_end + 1);
    for (int i = 0; i < proc0->int_registers[23]; ++i) {
        printf("%d ", *(int *) (proc0->ram + 59 + 4 + i * 8));
    }
    printf("\n took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
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
    run(proc0, true, true);
    save_state(proc0, "../state_files/7in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/7out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %f\n", proc0->assigned_task.program_end + 1,
           proc0->float_registers[28]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_8(processor_t *proc0) {
    clock_t begin_dirty = clock();
    proc0->float_registers[28] = -100;
    proc0->float_registers[25] = 1000;
    proc0->float_registers[26] = 1;
    proc0->int_registers[22] = 392;
    proc0->int_registers[23] = 396;
    assign_task(proc0, "../example_binaries/8.txt");
    run(proc0, true, true);
    save_state(proc0, "../state_files/8in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/8out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %f, %f\n", proc0->assigned_task.program_end + 1,
           *(float *) (proc0->ram + 49), *(float *) (proc0->ram + 53));
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_9(processor_t *proc0) {
    struct LL {
        uint8_t data[2];
        uint8_t padding[6]; // to match example
        uint8_t next[8]; // next is a long but due to no 64 bit we treat it as a padded int
    } element, list;
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 48;
    proc0->int_registers[23] = 64;
    memcpy(proc0->assigned_task.content + 6, &list, 16);
    memcpy(proc0->assigned_task.content + 22, &element, 16);
    assign_task(proc0, "../example_binaries/9.txt");
    run(proc0, true, true);
    save_state(proc0, "../state_files/9in.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/9out.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits\n", proc0->assigned_task.program_end + 1);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_10(processor_t *proc0) {
    clock_t begin_dirty = clock();
    assign_task(proc0, "../example_binaries/A.txt");
    run(proc0, true, true);
    save_state(proc0, "../state_files/Ain.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/Aout.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits\n", proc0->assigned_task.program_end + 1);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_11(processor_t *proc0) {
    int a[] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 100};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 464;
    proc0->int_registers[23] = 11;
    proc0->int_registers[24] = 11;
    assign_task(proc0, "../example_binaries/B.txt");
    memcpy(proc0->assigned_task.content + 58, a, 44);
    run(proc0, true, true);
    save_state(proc0, "../state_files/Bin.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/Bout.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: %d\n", proc0->assigned_task.program_end + 1,
           proc0->int_registers[22]);
    printf("took %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void ex_12(processor_t *proc0) {
    float a[] = {1, 1, 1, 1, 1, 1, 0, 1, 0};
    float dst[] = {0, 0, 0};
    float vector[] = {1, 2, 3};
    clock_t begin_dirty = clock();
    proc0->int_registers[22] = 640;
    proc0->int_registers[23] = 652;
    proc0->int_registers[24] = 688;
    assign_task(proc0, "../example_binaries/C.txt");
    memcpy(proc0->assigned_task.content + 80, dst, 12);
    memcpy(proc0->assigned_task.content + 92, a, 36);
    memcpy(proc0->assigned_task.content + 128, vector, 12);
    run(proc0, true, true);
    save_state(proc0, "../state_files/Cin.bin");
    clock_t begin_clean = clock();
    run(proc0, false, false);
    clock_t end_clean = clock();
    double time_spent_clean = (double) (end_clean - begin_clean) / CLOCKS_PER_SEC;
    save_state(proc0, "../state_files/Cout.bin");
    clock_t end_dirty = clock();
    double time_spent_dirty = (double) (end_dirty - begin_dirty) / CLOCKS_PER_SEC;
    printf("read file, program ends after %hu bits, answer: ", proc0->assigned_task.program_end + 1);
    for (int i = 0; i < 3; ++i) {
        printf("%f ", *(float *) (&proc0->ram[80] + i * 4));
    }
    printf("\ntook %f to run, %f to setup, total %f (all times in seconds)", time_spent_clean,
           time_spent_dirty - time_spent_clean, time_spent_dirty);
}

void generate_all_state_files(processor_t *proc)
{
    ex_1(proc);
    ex_2(proc);
    ex_3(proc);
    ex_4(proc);
    ex_5(proc);
    ex_6(proc);
    ex_7(proc);
    ex_8(proc);
    ex_9(proc);
    ex_10(proc);
    ex_11(proc);
    ex_12(proc);
}