/* 2.4 struct pkt {ch1; ch2[2]; ch3} filled from one int, then reassembled.
 * Run: ./2.4 305419896  ->  78 56 34 12 | Reassembled: 305419896 Match: yes
 */
#include <stdio.h>
#include <stdlib.h>
struct pkt { char ch1; char ch2[2]; char ch3; };
int main(int c, char **v) {
    int n = c == 2 ? atoi(v[1]) : 0x12345678;
    struct pkt p = { n & 0xFF, { (n >> 8) & 0xFF, (n >> 16) & 0xFF }, (n >> 24) & 0xFF };
    printf("%02X %02X %02X %02X\n", (unsigned char)p.ch1, (unsigned char)p.ch2[0],
           (unsigned char)p.ch2[1], (unsigned char)p.ch3);
    unsigned r = (unsigned char)p.ch1 | (unsigned char)p.ch2[0] << 8
               | (unsigned char)p.ch2[1] << 16 | (unsigned char)p.ch3 << 24;
    printf("Reassembled: %u Match: %s\n", r, r == (unsigned)n ? "yes" : "no");
}
