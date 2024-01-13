#include "extern_function_caller.h"

void dispatch_call(char *func_name, processor_t *proc)
{
    // append the corect call procedure to the if/else chain
    if (strcmp("cfunc", func_name) == 0)
    {
        proc->int_registers[22] = cfunc(proc->int_registers[22], proc->int_registers[23], proc->int_registers[24]);
    } else
    {
        fprintf(stderr, "[ERROR] Could not find %s in library, terminating program...", func_name);
        exit(1);
    }
}