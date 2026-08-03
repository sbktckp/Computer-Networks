/*
 * 2.3 Extract Each Byte of a Number into Separate char Variables
 *
 * Aim: take an int, pull out its 4 constituent bytes into 4 named char
 *      variables, print each.
 *
 * Method: an int on this machine is 4 bytes sitting contiguously in
 *         memory. Reinterpreting its address as unsigned char* turns
 *         indexing into direct byte access, so byte0..byte3 are just
 *         named reads off that reinterpreted pointer, in whatever
 *         order the host's endianness actually stores them, no shifting
 *         or masking arithmetic required. unsigned char is used for the
 *         read so the bit pattern prints as 0-255 instead of being sign
 *         extended through a plain (possibly signed) char.
 *
 * Run:    ./bin/2.3_byte_extraction 305419896
 * Output: Number       : 305419896 (0x12345678)
 *         byte0 (LSB@offset0) = 0x78
 *         byte1                = 0x56
 *         byte2                = 0x34
 *         byte3 (MSB@offset3) = 0x12
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int number = (argc == 2) ? (int)strtol(argv[1], NULL, 0) : 0x12345678;

    unsigned char *p = (unsigned char *)&number;
    unsigned char byte0 = p[0];
    unsigned char byte1 = p[1];
    unsigned char byte2 = p[2];
    unsigned char byte3 = p[3];

    printf("Number       : %d (0x%08X)\n", number, (unsigned)number);
    printf("byte0 (offset0) = 0x%02X\n", byte0);
    printf("byte1 (offset1) = 0x%02X\n", byte1);
    printf("byte2 (offset2) = 0x%02X\n", byte2);
    printf("byte3 (offset3) = 0x%02X\n", byte3);

    return 0;
}
