/*
 * 2.4 Store a Number Across struct pkt, Print It, Then Reassemble
 * Aim: split a number's bytes across struct pkt { char ch1; char ch2[2];
 *      char ch3; }, print them, then fold them back into the original
 *      number and confirm the match.
 *
 * Input  : ./2.4_struct_pkt_reassemble 305419896   (0x12345678)
 * Output : 78 56 34 12
 *          Reassembled: 305419896 Match: yes
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
