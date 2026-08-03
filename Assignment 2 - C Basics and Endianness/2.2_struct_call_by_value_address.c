/*
 * 2.2 Struct, Nested Struct, Call-by-Value vs Call-by-Address
 *
 * Aim: populate struct student_info (which embeds struct dob), pass it
 *      to one function by value and another by address, print members
 *      from inside each to show which mutation survives the call.
 *
 * Method: call-by-value copies the whole struct onto the callee's stack
 *         frame, name array included, so any write inside modify_by_value
 *         is invisible to main once the function returns. modify_by_address
 *         instead receives a struct student_info*, so it writes through the
 *         same memory main is holding, and the change is visible after the
 *         call. Printing CGPA and roll_no from both callees, and again from
 *         main afterwards, makes the difference concrete rather than a rule
 *         to memorise.
 *
 * Run:    ./bin/2.2_struct_call_by_value_address
 * Output: Original         : roll=101 name=Aditi Sharma cgpa=8.75 dob=14/03/2005
 *         Inside by_value  : roll=999 name=CHANGED cgpa=0.00 dob=1/1/1999 (local only)
 *         Inside by_address: roll=101 name=Aditi Sharma cgpa=9.10 dob=15/03/2005
 *         After call       : roll=101 name=Aditi Sharma cgpa=9.10 dob=15/03/2005
 */
#include <stdio.h>
#include <string.h>

struct dob {
    int day, month, year;
};

struct student_info {
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

static void print_student(const char *label, const struct student_info *s) {
    printf("%-18s: roll=%d name=%s cgpa=%.2f dob=%d/%d/%d\n",
           label, s->roll_no, s->name, s->CGPA,
           s->age.day, s->age.month, s->age.year);
}

/* Whole struct copied in; every write here dies with the function frame. */
void modify_by_value(struct student_info s) {
    s.roll_no = 999;
    strcpy(s.name, "CHANGED");
    s.CGPA = 0.0f;
    s.age = (struct dob){1, 1, 1999};
    print_student("Inside by_value", &s);
}

/* Only an address crosses the call, so writes land in the caller's struct. */
void modify_by_address(struct student_info *s) {
    s->CGPA = 9.10f;
    s->age.day = 15;
    print_student("Inside by_address", s);
}

int main(void) {
    struct student_info stu = {
        .roll_no = 101,
        .name = "Aditi Sharma",
        .CGPA = 8.75f,
        .age = {14, 3, 2005}
    };

    print_student("Original", &stu);
    modify_by_value(stu);            /* stu itself cannot change */
    modify_by_address(&stu);         /* stu is now actually updated */
    print_student("After call", &stu);

    return 0;
}
