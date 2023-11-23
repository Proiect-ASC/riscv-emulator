#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define BUFFER_SIZE 8 * 1024
bool buffer[BUFFER_SIZE];

void load_buffer() {
    // ADD 1    3
    // 10 0001 0011
    //
    // SUB 5    1
    // 01 0101 0001
    //
    // 0100 0100 1110 0101 0001
    
    // ADD
    buffer[0] = 0;
    buffer[1] = 1;

    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 1;

    buffer[6] = 0;
    buffer[7] = 0;
    buffer[8] = 1;
    buffer[9] = 1;

    // SUB
    buffer[10] = 1;
    buffer[11] = 0;

    buffer[12] = 0;
    buffer[13] = 1;
    buffer[14] = 0;
    buffer[15] = 1;

    buffer[16] = 0;
    buffer[17] = 0;
    buffer[18] = 0;
    buffer[19] = 1;
}

int read_byte(int location) {
    int result = 0;
    const int BYTE = 4; // testing purposes
    // TODO: Change to 8 & Add Bounds
    for (int i = 0; i < BYTE; i++) {
        result = (result << 1) + buffer[location + i];
    }
    return result;
}

typedef enum Command {
    ADD = 0b10,
    SUB = 0b01,
} Command;

int main() {
    puts("Emulating...");
    unsigned int location = 0;
    unsigned int command = 1;
    // TODO: remove location
    while (location < BUFFER_SIZE && location < 20) {
        printf("[INFO]: Looking at bit %d\n", location);
        command = 2 * command + buffer[location];
        if ((command >> 1) == ADD) {
            printf("[INFO]: Adding at: %d\n", location);
            printf("[INFO]: Command code: %d\n", command);

            int a = read_byte(location + 1);
            printf("First argument is: %d\n", a);
            assert(a == 1);
            location += 4;

            int b = read_byte(location + 1);
            printf("Second argument is: %d\n", b);
            assert(b == 3);
            location += 4;
            location++;

            // RESET
            command = 1;
            printf("[INFO]: result:%d\n", a + b);
        } else if ((command >> 1) == SUB) {
            puts("todo");

            // RESET
            command = 1;
        }
        location++;
    }
    return 0;
}
