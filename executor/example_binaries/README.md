The files contained in this folder represent the binaries resulted from assembling the 12 examples provided in the examples folder.

IMPORTANT NOTE: The first 24 bits represent a header, and are not counted by the program counter; program counter begins with the 24th bit, counting from 0. In other words, address 0 does not actually start with bit no. 0, but with bit no. 24.

The encoding format is the following:
- bits[0-7]: Unsigned 8-bit number representing the ammount of bits that should be ignored at the end of the binary (remainder);
- bits[8 - 23]: Unsigned 16-bit number representing start of program address offset (ps);
- bits[24 - (ps-1)]: Program data, if any.
- bits[ps - (EOF-remainder-1)]: Encoded assembly instructions.
- bits[(EOF-remainder) - EOF]: This portion of the binary should be ignored.
