/*
 * 2.5 Detect Host Endianness, Print Byte Layout, Convert Endianness
 *
 * Aim: check whether the machine is little-endian or big-endian, print
 *      the byte layout of a user-supplied number, then produce its
 *      opposite-endian representation.
 *
 * Method: a union overlays an int and a 4-byte char array on the same
 *         storage. Writing 1 to the int and reading char[0] answers the
 *         endianness question directly: if the least significant byte
 *         landed at the lowest address, char[0] is 1 and the host is
 *         little-endian, otherwise it is big-endian. Byte-swapping to
 *         the opposite representation is a single expression built from
 *         four shifted-and-masked bytes recombined in reverse order, the
 *         same operation network code uses (it is what htonl/ntohl do
 *         under the hood for 32-bit words).
 *
 * Run:    ./bin/2.5_endianness_check 305419896
 * Output: Host endianness : Little Endian
 *         Number          : 305419896 (0x12345678)
 *         Byte[0..3]      : 78 56 34 12
 *         Swapped         : 2018915346 (0x78563412)
 */
#include <stdio.h>
#include <stdlib.h>

static int host_is_little_endian(void) {
    union { int i; unsigned char c[sizeof(int)]; } probe = { .i = 1 };
    return probe.c[0] == 1;
}

static unsigned int swap_endian32(unsigned int n) {
    return  (n << 24)
          | ((n <<  8) & 0x00FF0000)
          | ((n >>  8) & 0x0000FF00)
          | (n >> 24);
}

int main(int argc, char *argv[]) {
    unsigned int number = (argc == 2) ? (unsigned)strtoul(argv[1], NULL, 0)
                                       : 0x12345678U;

    int little = host_is_little_endian();
    printf("Host endianness : %s Endian\n", little ? "Little" : "Big");
    printf("Number          : %u (0x%08X)\n", number, number);

    unsigned char *p = (unsigned char *)&number;
    printf("Byte[0..3]      : %02X %02X %02X %02X\n", p[0], p[1], p[2], p[3]);

    unsigned int swapped = swap_endian32(number);
    printf("Swapped         : %u (0x%08X)\n", swapped, swapped);

    return 0;
}
