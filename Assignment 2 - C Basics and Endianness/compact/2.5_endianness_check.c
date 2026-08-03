/* 2.5 Detect endianness via union, print+swap bytes via loop.
 * Run: ./2.5 305419896  ->  Little Endian | 78 56 34 12 | Swapped: 2018915346
 */
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    unsigned n = argc == 2 ? (unsigned)atol(argv[1]) : 0x12345678U;
    union { int i; unsigned char c[4]; } u = { .i = 1 };
    printf("%s Endian\n", u.c[0] == 1 ? "Little" : "Big");
    unsigned char *b = (unsigned char *)&n;
    for (size_t i = 0; i < sizeof(n); i++) printf("%02X ", b[i]);
    printf("\n");
    for (size_t i = 0; i < sizeof(n) / 2; i++) {
        unsigned char t = b[i]; b[i] = b[sizeof(n) - 1 - i]; b[sizeof(n) - 1 - i] = t;
    }
    printf("Swapped: %u\n", n);
}
