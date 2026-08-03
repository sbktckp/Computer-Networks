/*
 * 2.4 Store a Number Across struct pkt, Print It, Then Reassemble
 *
 * Aim: given struct pkt { char ch1; char ch2[2]; char ch3; }, distribute
 *      a 4-byte number across its members, print each member, then
 *      aggregate them back into the original number and confirm the match.
 *
 * Method: struct pkt has exactly 4 char-sized slots (ch1, ch2[0], ch2[1],
 *         ch3), which is precisely 4 bytes, the width of an int on this
 *         platform. A single byte-order-independent split with shifts and
 *         masks fills the four slots; reassembly runs the same shifts in
 *         reverse and ORs them together, so the code makes no assumption
 *         about the host's endianness even though question 2.5 studies it
 *         directly. Fields are read into unsigned char first so the shift
 *         reassembly cannot be corrupted by sign extension from a negative
 *         char value.
 *
 * Run:    ./bin/2.4_struct_pkt_reassemble 305419896
 * Output: Original number : 305419896 (0x12345678)
 *         pkt.ch1         = 0x78
 *         pkt.ch2[0]      = 0x56
 *         pkt.ch2[1]      = 0x34
 *         pkt.ch3         = 0x12
 *         Reassembled     : 305419896 (0x12345678)
 *         Match           : yes
 */
#include <stdio.h>
#include <stdlib.h>

struct pkt {
    char ch1;
    char ch2[2];
    char ch3;
};

int main(int argc, char *argv[]) {
    int number = (argc == 2) ? (int)strtol(argv[1], NULL, 0) : 0x12345678;

    struct pkt p;
    p.ch1    = (char)( number        & 0xFF);
    p.ch2[0] = (char)((number >>  8) & 0xFF);
    p.ch2[1] = (char)((number >> 16) & 0xFF);
    p.ch3    = (char)((number >> 24) & 0xFF);

    printf("Original number : %d (0x%08X)\n", number, (unsigned)number);
    printf("pkt.ch1         = 0x%02X\n", (unsigned char)p.ch1);
    printf("pkt.ch2[0]      = 0x%02X\n", (unsigned char)p.ch2[0]);
    printf("pkt.ch2[1]      = 0x%02X\n", (unsigned char)p.ch2[1]);
    printf("pkt.ch3         = 0x%02X\n", (unsigned char)p.ch3);

    unsigned int reassembled =
          ((unsigned char)p.ch1)
        | ((unsigned char)p.ch2[0] <<  8)
        | ((unsigned char)p.ch2[1] << 16)
        | ((unsigned char)p.ch3    << 24);

    printf("Reassembled     : %u (0x%08X)\n", reassembled, reassembled);
    printf("Match           : %s\n",
           (reassembled == (unsigned int)number) ? "yes" : "no");

    return 0;
}
