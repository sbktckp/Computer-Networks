/* 2.1 Swap via pointer, ints from argv.
 * Run: ./2.1 10 25  ->  Before: 10 25 | After: 25 10
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
