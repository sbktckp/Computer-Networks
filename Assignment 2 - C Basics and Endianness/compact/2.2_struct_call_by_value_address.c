/* 2.2 struct w/ nested struct, call-by-value vs call-by-address.
 * Run: ./2.2
 * Output: Original: 101 Aditi 8.75 | ByValue: 999 CHANGED 0.00 | ByAddr: 101 Aditi 9.10
 */
#include <stdio.h>
#include <string.h>
struct dob { int d, m, y; };
struct student { int roll; char name[50]; float cgpa; struct dob dob; };

void by_value(struct student s) {
    s.roll = 999; strcpy(s.name, "CHANGED"); s.cgpa = 0;
    printf("ByValue: %d %s %.2f\n", s.roll, s.name, s.cgpa);
}
void by_addr(struct student *s) {
    s->cgpa = 9.10f;
    printf("ByAddr: %d %s %.2f\n", s->roll, s->name, s->cgpa);
}
int main(void) {
    struct student stu = {101, "Aditi", 8.75f, {14, 3, 2005}};
    printf("Original: %d %s %.2f\n", stu.roll, stu.name, stu.cgpa);
    by_value(stu);
    by_addr(&stu);
    printf("After: %d %s %.2f\n", stu.roll, stu.name, stu.cgpa);
}
