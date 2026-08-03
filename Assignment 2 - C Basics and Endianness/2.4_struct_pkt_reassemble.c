/*
 * 2.4 Store a Number Across struct pkt, Print It, Then Reassemble
 *
 * Aim: given struct pkt { char ch1; char ch2[2]; char ch3; }, distribute
 *      a 4-byte number across its members, print each member, then
 *      reaggregate them into the original number and confirm the match.
 *
 * Design notes:
 *   - The struct's shape is fixed by the question (3 named members, one
 *     of which is itself a 2-element array). C has no reflection over
 *     struct members, so filling ch1/ch2[0]/ch2[1]/ch3 is 4 direct
 *     assignments, not something a loop could express more clearly.
 *   - Reassembly, by contrast, is naturally a fold over 4 byte
 *     positions, so it is written as a loop over an array of the same
 *     4 values (taken from the struct) rather than as 4 repeated OR
 *     expressions, this is what actually scales if the packet grew
 *     another byte field.
 *   - Every byte is read through unsigned char before shifting, so a
 *     byte with its high bit set does not sign-extend and corrupt the
 *     upper bits of the reassembled value.
 *
 * Run:    ./2.4_struct_pkt_reassemble 305419896
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

    /* Fold the 4 bytes back into one value; a loop scales if the
     * packet grows another byte field, four hand-written ORs don't. */
    unsigned char ordered[] = {
        (unsigned char)p.ch1, (unsigned char)p.ch2[0],
        (unsigned char)p.ch2[1], (unsigned char)p.ch3
    };
    unsigned int reassembled = 0;
    for (size_t i = 0; i < sizeof(ordered); i++)
        reassembled |= (unsigned int)ordered[i] << (8 * i);

    printf("Reassembled     : %u (0x%08X)\n", reassembled, reassembled);
    printf("Match           : %s\n",
           (reassembled == (unsigned int)number) ? "yes" : "no");

    return 0;
}
