#include "processor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void ex_1(processor_t *proc0) {
    char *str = "tibi o suge rau";
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

int main() {
    processor_t proc0;
    ex_1(&proc0);

    return 0;
}
