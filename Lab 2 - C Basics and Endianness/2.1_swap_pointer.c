/*
 * 2.1 Swap Two Variables Using a Function and Pointers
 *
 * Aim: read two integers from the command line, swap them through a
 *      function that receives their addresses, print before and after.
 *
 * Design notes:
 *   - strtol() over atoi(): atoi() has no way to report "this wasn't a
 *     number", it silently returns 0. strtol() reports failure through
 *     endptr, so a bad argument is caught instead of misread as 0.
 *   - swap() takes int* and returns void. Returning the swapped values
 *     instead would need two return channels (a struct, or output
 *     params anyway) for no benefit; pointers are the natural fit for
 *     "modify what the caller already has."
 *
 * Run:    ./2.1_swap_pointer 10 25
 * Output: Before swap: a = 10, b = 25
 *         After swap:  a = 25, b = 10
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Parses one command-line argument as a base-10 int.
 * Returns 1 and writes *out on success, 0 on a malformed argument. */
static int parse_int(const char *s, int *out) {
    errno = 0;
    char *end;
    long v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || errno == ERANGE || v < INT_MIN || v > INT_MAX)
        return 0;
    *out = (int)v;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <int_a> <int_b>\n", argv[0]);
        return 1;
    }

    int a, b;
    if (!parse_int(argv[1], &a) || !parse_int(argv[2], &b)) {
        fprintf(stderr, "error: both arguments must be valid integers\n");
        return 1;
    }

    printf("Before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("After swap:  a = %d, b = %d\n", a, b);

    return 0;
}
