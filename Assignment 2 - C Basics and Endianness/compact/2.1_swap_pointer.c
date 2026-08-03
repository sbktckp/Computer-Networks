/*
 * 2.1 Swap Two Variables Using Pointers and Command Line Arguments
 * Aim: swap two integers, passed via argv, using a function that takes
 *      their addresses.
 *
 * Input  : ./2.1_swap_pointer 10 25
 * Output : Before: 10 25
 *          After: 25 10
 */
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

int main(int argc, char **argv) {
    if (argc != 3) { fprintf(stderr, "usage: %s a b\n", argv[0]); return 1; }
    int a = atoi(argv[1]), b = atoi(argv[2]);
    printf("Before: %d %d\n", a, b);
    swap(&a, &b);
    printf("After: %d %d\n", a, b);
}
