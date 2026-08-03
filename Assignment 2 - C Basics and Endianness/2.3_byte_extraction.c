/*
 * 2.3 Extract Each Byte of a Number into Separate char Variables
 *
 * Aim: take a number, pull out its constituent bytes, store each in its
 *      own char variable, print them.
 *
 * Design notes:
 *   - "separate character variables" literally means named char storage,
 *     not just an array subscript, so the bytes are captured into a
 *     fixed-size array of char (bytes[0..sizeof(int)-1]) which gives each
 *     one its own storage location, then the array is looped over to
 *     print, the loop is the natural tool here since "extract every
 *     byte of a value" is inherently a bounded iteration over
 *     sizeof(int), not four special cases.
 *   - sizeof(int) drives the loop bound instead of a hardcoded 4, so
 *     the same code is correct on any platform's int width without
 *     being rewritten.
 *   - unsigned char for the read, to avoid sign extension turning a
 *     high-bit byte like 0xFF into a negative int when printed.
 *
 * Run:    ./2.3_byte_extraction 305419896
 * Output: Number : 305419896 (0x12345678)
 *         byte[0] (offset 0) = 0x78
 *         byte[1] (offset 1) = 0x56
 *         byte[2] (offset 2) = 0x34
 *         byte[3] (offset 3) = 0x12
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int number = (argc == 2) ? (int)strtol(argv[1], NULL, 0) : 0x12345678;

    unsigned char bytes[sizeof(number)];
    const unsigned char *src = (const unsigned char *)&number;
    for (size_t i = 0; i < sizeof(number); i++)
        bytes[i] = src[i];

    printf("Number : %d (0x%08X)\n", number, (unsigned)number);
    for (size_t i = 0; i < sizeof(bytes); i++)
        printf("byte[%zu] (offset %zu) = 0x%02X\n", i, i, bytes[i]);

    return 0;
}
