#include "processor.h"
#include <stdio.h>
#include <string.h>

void ex_1(processor_t *proc0) {
    char *str = "tibi o suge rau";
    proc0->int_registers[22] = 192;
    assign_task(proc0, "../example_binaries/1.txt");
    memcpy(proc0->assigned_task.content + 24, str, strlen(str) + 1);
    save_state(proc0, "../state_files/1.in");
    run(proc0);
    save_state(proc0, "../state_files/1.out");
    printf("read file, program ends after %hu bits, answer: %d", proc0->assigned_task.program_end + 1, proc0->int_registers[22]);
}

int main() {
    processor_t proc0;
    ex_1(&proc0);

    return 0;
}
