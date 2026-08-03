/* 2.5 Detect endianness via union, print bytes, swap to opposite endian.
 * Run: ./2.5 305419896  ->  Little Endian | 78 56 34 12 | Swapped: 2018915346
 */
#include <stdio.h>
#include <stdlib.h>
int main(int c, char **v) {
    unsigned n = c == 2 ? (unsigned)atol(v[1]) : 0x12345678U;
    union { int i; unsigned char c[4]; } u = { .i = 1 };
    printf("%s Endian\n", u.c[0] == 1 ? "Little" : "Big");
    unsigned char *p = (unsigned char *)&n;
    printf("%02X %02X %02X %02X\n", p[0], p[1], p[2], p[3]);
    unsigned s = n << 24 | (n << 8 & 0xFF0000) | (n >> 8 & 0xFF00) | n >> 24;
    printf("Swapped: %u\n", s);
}
