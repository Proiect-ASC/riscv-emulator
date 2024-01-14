#include "processor.h"
#include "generate_state_files.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        fprintf(stderr, "\n[Error] No arguments passed, terminating program...");
        exit(1);
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("\nUsage:\n\texecutor --help -> display this message;\n\texecutor <path> -> run binary at <path>;\n\texecutor <path0> --load-statefile <path1> -> run binary at"
                   " <path0> after loading statefile at <path1>;\n\texecutor <path0> --generate-statefiles <path1> <path2> -> run binary at <path0> (must be able to run without a statefile!) and generate input and output statefiles"
                    " at <path1> respectively <path2>;\n");
            exit(1);
        }
        printf("\n[Status] Creating virtual processor...");
        printf("\n[Status] Done!");
        processor_t proc0;
        printf("\n[Status] Assigning binary at \"%s\" as virtual processor's task...", argv[1]);
        assign_task(&proc0, argv[1]);
        printf("\n[Status] Done!");
        printf("\n[Status] Running binary at \"%s\" without loading from nor saving a statefile...\n\n", argv[1]);
        run(&proc0, true, false);
        printf("\n[Status] Done!");
        printf("\n[Status] Execution finished.");
        return 0;
    }
    else if(argc == 3)
    {
        fprintf(stderr, "\n[Error] Expecting 2, 4 or 5 program arguments, 3 found, terminating program...");
        exit(1);
    }
    else {
        if (strcmp(argv[2], "--load-statefile") != 0 && strcmp(argv[2], "--generate-statefiles") != 0) {
            fprintf(stderr,
                    "\n[Error] Unrecognized flag %s, expecting --load-statefile or --generate-statefiles, terminating program...",
                    argv[2]);
            exit(1);
        } else {
            if (argc == 4) {
                if (strcmp(argv[2], "--load-statefile") != 0)
                {
                    fprintf(stderr,
                            "\n[Error] Too few program arguments for --generate-statefiles, found 4, expecting 5, terminating program...");
                    exit(1);
                }
                printf("\n[Status] Creating virtual processor...");
                printf("\n[Status] Done!");
                processor_t proc0;
                printf("\n[Status] Assigning binary at \"%s\" as virtual processor's task...", argv[1]);
                assign_task(&proc0, argv[1]);
                printf("\n[Status] Done!");
                printf("\n[Status] Running binary at \"%s\" with loading from statefile at \"%s\"...\n\n", argv[1], argv[3]);
                load_state(&proc0, argv[3]);
                run(&proc0, false, false);
                printf("\n[Status] Done!");
                printf("\n[Status] Execution finished.");
                return 0;
            } else {
                if (argc == 5)
                {
                    if (strcmp(argv[2], "--generate-statefiles") != 0)
                    {
                        fprintf(stderr,
                                "\n[Error] Too many program arguments for --load-statefile, found 5, expecting 4, terminating program...");
                        exit(1);
                    }
                    printf("\n[Status] Creating virtual processor...");
                    printf("\n[Status] Done!");
                    processor_t proc0;
                    printf("\n[Status] Assigning binary at \"%s\" as virtual processor's task...", argv[1]);
                    assign_task(&proc0, argv[1]);
                    printf("\n[Status] Done!");
                    printf("\n[Status] Running binary at \"%s\" with generating statefiles at \"%s\" (input) and \"%s\" (output)\n\n", argv[1], argv[3], argv[4]);
                    run(&proc0, true, true);
                    save_state(&proc0, argv[3]);
                    run(&proc0, false, false);
                    save_state(&proc0, argv[4]);
                    printf("\n[Status] Done!");
                    printf("\n[Status] Execution finished.");
                    return 0;
                }
                else
                {
                    fprintf(stderr, "\n[Error] Too many program arguments for flag %s terminating program...", argv[2]);
                    exit(1);
                }
            }
        }
    }
}
//    just for presentation purpouses
/*int main() {
    processor_t proc0;
    generate_all_state_files(&proc0);
    return 0;
}*/

