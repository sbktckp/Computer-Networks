/* 2.4 struct pkt {ch1; ch2[2]; ch3} filled from an int, reassembled via loop.
 * Run: ./2.4 305419896  ->  78 56 34 12 | Reassembled: 305419896 Match: yes
 */
#include <stdio.h>
#include <stdlib.h>
struct pkt { char ch1; char ch2[2]; char ch3; };
int main(int argc, char **argv) {
    int n = argc == 2 ? atoi(argv[1]) : 0x12345678;
    struct pkt p = { n & 0xFF, { (n >> 8) & 0xFF, (n >> 16) & 0xFF }, (n >> 24) & 0xFF };
    unsigned char bytes[] = { p.ch1, p.ch2[0], p.ch2[1], p.ch3 };
    unsigned r = 0;
    for (size_t i = 0; i < sizeof(bytes); i++) {
        printf("%02X ", bytes[i]);
        r |= (unsigned)bytes[i] << (8 * i);
    }
    printf("\nReassembled: %u Match: %s\n", r, r == (unsigned)n ? "yes" : "no");
}
