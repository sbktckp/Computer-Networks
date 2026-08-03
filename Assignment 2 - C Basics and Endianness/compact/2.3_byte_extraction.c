/*
 * 2.3 Extract Each Byte of a Number into Separate char Variables
 * Aim: pull out every byte of an int by reinterpreting its address as
 *      unsigned char*, looping over sizeof(int) positions.
 *
 * Input  : ./2.3_byte_extraction 305419896   (0x12345678)
 * Output : 78 56 34 12
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int n = argc == 2 ? atoi(argv[1]) : 0x12345678;
    unsigned char *p = (unsigned char *)&n;
    for (size_t i = 0; i < sizeof(n); i++) printf("%02X ", p[i]);
    printf("\n");
}
