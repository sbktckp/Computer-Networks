/*
 * 2.1 Swap Using Pointer and Command Line Arguments
 *
 * Aim: read two integers from argv, swap them through a function that
 *      takes their addresses, print before and after.
 *
 * Method: the swap function only ever sees two int* parameters, so it
 *         has no idea whether they point into the stack, the heap, or
 *         two unrelated arrays. That is the entire point of passing by
 *         address: swap(&a, &b) rewrites the caller's variables without
 *         returning anything.
 *
 * Run:    ./bin/2.1_swap_pointer 10 25
 * Output: Before swap: a = 10, b = 25
 *         After swap:  a = 25, b = 10
 */
#include <stdio.h>
#include <stdlib.h>

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <int_a> <int_b>\n", argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    printf("Before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("After swap:  a = %d, b = %d\n", a, b);

    return 0;
}
