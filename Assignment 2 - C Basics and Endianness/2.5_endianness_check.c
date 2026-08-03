/*
 * 2.5 Detect Host Endianness, Print Byte Layout, Convert Endianness
 *
 * Aim: check whether the machine is little-endian or big-endian, print
 *      the byte layout of a user-supplied number, produce its
 *      opposite-endian representation.
 *
 * Design notes:
 *   - Endianness detection is a single comparison (does byte 0 hold the
 *     least-significant byte?), so it stays a one-line check via a
 *     union, not a loop — there's nothing to iterate over.
 *   - Printing and swapping bytes, on the other hand, are naturally
 *     iteration over sizeof(int) byte positions. The swap is written
 *     as a loop that reverses byte order for any width, which is both
 *     clearer than a hand-written 4-term OR expression and correct if
 *     compiled where int isn't 4 bytes, unlike a fixed bitmask version.
 *
 * Run:    ./2.5_endianness_check 305419896
 * Output: Host endianness : Little Endian
 *         Number          : 305419896 (0x12345678)
 *         Byte layout     : [0]=0x78 [1]=0x56 [2]=0x34 [3]=0x12
 *         Swapped         : 2018915346 (0x78563412)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int host_is_little_endian(void) {
    union { int i; unsigned char c[sizeof(int)]; } probe = { .i = 1 };
    return probe.c[0] == 1;
}

/* Reverses byte order of an arbitrary-width integer in place. */
static void reverse_bytes(void *value, size_t width) {
    unsigned char *b = (unsigned char *)value;
    for (size_t i = 0; i < width / 2; i++) {
        unsigned char tmp = b[i];
        b[i] = b[width - 1 - i];
        b[width - 1 - i] = tmp;
    }
}

int main(int argc, char *argv[]) {
    unsigned int number = (argc == 2) ? (unsigned)strtoul(argv[1], NULL, 0)
                                       : 0x12345678U;

    int little = host_is_little_endian();
    printf("Host endianness : %s Endian\n", little ? "Little" : "Big");
    printf("Number          : %u (0x%08X)\n", number, number);

    unsigned char *bytes = (unsigned char *)&number;
    printf("Byte layout     :");
    for (size_t i = 0; i < sizeof(number); i++)
        printf(" [%zu]=0x%02X", i, bytes[i]);
    printf("\n");

    unsigned int swapped = number;
    reverse_bytes(&swapped, sizeof(swapped));
    printf("Swapped         : %u (0x%08X)\n", swapped, swapped);

    return 0;
}
