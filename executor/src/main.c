#include "processor.h"
#include <stdio.h>

int main() {
    binary file = load_executable("../example_binaries/1.txt");
    processor_t proc0;
    run(&proc0, &file);
    printf("read file, program ends after %hu bits", file.program_end);
    return 0;
}
