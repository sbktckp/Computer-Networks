/* 2.3 Extract bytes of an int via a loop over sizeof(int).
 * Run: ./2.3 305419896  ->  78 56 34 12
 */
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int n = argc == 2 ? atoi(argv[1]) : 0x12345678;
    unsigned char *p = (unsigned char *)&n;
    for (size_t i = 0; i < sizeof(n); i++) printf("%02X ", p[i]);
    printf("\n");
}
