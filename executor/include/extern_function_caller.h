#ifndef EXECUTOR_EXTERN_FUNCTION_CALLER_H
#define EXECUTOR_EXTERN_FUNCTION_CALLER_H

#include "libraries.h"
#include "processor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void dispatch_call(char *func_name, processor_t *proc);

#endif //EXECUTOR_EXTERN_FUNCTION_CALLER_H
