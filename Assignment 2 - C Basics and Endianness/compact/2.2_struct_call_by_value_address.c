/* 2.2 struct with nested struct, call-by-value vs call-by-address.
 * Run: ./2.2
 * Output: Original: 101 Aditi 8.75 | ByValue(local): 999 CHANGED 0.00 | ByAddr: 101 Aditi 9.10 | After: 101 Aditi 9.10
 */
#include <stdio.h>
#include <string.h>
struct dob { int d, m, y; };
struct student_info { int roll_no; char name[50]; float CGPA; struct dob age; };

void by_value(struct student_info s) {
    s.roll_no = 999; strcpy(s.name, "CHANGED"); s.CGPA = 0;
    printf("ByValue(local): %d %s %.2f\n", s.roll_no, s.name, s.CGPA);
}
void by_addr(struct student_info *s) {
    s->CGPA = 9.10f;
    printf("ByAddr: %d %s %.2f\n", s->roll_no, s->name, s->CGPA);
}
int main(void) {
    struct student_info stu = {101, "Aditi", 8.75f, {14, 3, 2005}};
    printf("Original: %d %s %.2f\n", stu.roll_no, stu.name, stu.CGPA);
    by_value(stu);
    by_addr(&stu);
    printf("After: %d %s %.2f\n", stu.roll_no, stu.name, stu.CGPA);
}
