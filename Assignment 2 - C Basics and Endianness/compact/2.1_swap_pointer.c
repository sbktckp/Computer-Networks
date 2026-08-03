/* 2.1 Swap via pointer, values from argv.
 * Run: ./2.1 10 25  ->  Before: 10 25 | After: 25 10
 */
#include <stdio.h>
#include <stdlib.h>
void swap(int *x, int *y) { int t = *x; *x = *y; *y = t; }
int main(int c, char **v) {
    int a = c == 3 ? atoi(v[1]) : 10, b = c == 3 ? atoi(v[2]) : 25;
    printf("Before: %d %d\n", a, b);
    swap(&a, &b);
    printf("After: %d %d\n", a, b);
}
