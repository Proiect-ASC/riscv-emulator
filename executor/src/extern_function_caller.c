#include "extern_function_caller.h"

void dispatch_call(char *func_name, processor_t *proc) {
    // append the corect call procedure to the if/else chain
    if (strcmp("cfunc", func_name) == 0) {
        proc->int_registers[A0] = cfunc(proc->int_registers[A0], proc->int_registers[A1], proc->int_registers[A2]);
    } else {
        fprintf(stderr, "[ERROR] Could not find %s in library, terminating program...", func_name);
        exit(1);
    }
}