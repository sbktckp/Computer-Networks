/* 2.3 Extract 4 bytes of an int into separate char vars, no loop.
 * Run: ./2.3 305419896  ->  78 56 34 12
 */
#include <stdio.h>
#include <stdlib.h>
int main(int c, char **v) {
    int n = c == 2 ? atoi(v[1]) : 0x12345678;
    unsigned char *p = (unsigned char *)&n;
    unsigned char b0 = p[0], b1 = p[1], b2 = p[2], b3 = p[3];
    printf("%02X %02X %02X %02X\n", b0, b1, b2, b3);
}
