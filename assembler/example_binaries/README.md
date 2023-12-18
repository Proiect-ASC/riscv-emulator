The files contained in this folder represent the binaries resulted from assembling the 12 examples provided in the examples folder.

The encoding format is the following:
- bits[0-7]: Unsigned 8 bit number representing the ammount of bits that should be ignored at the end of the binary (remainder);
- bits[8 - 23]: Unsigned 16 bit number representing start of program address offset (ps);
- bits[23 - (ps-1)]: Program data, if any.
- bits[ps - (EOF-remainder-1)]: Encoded assembly instructions.
- bits[(EOF-remainder) - EOF]: This portion of the binary should be ignored.
