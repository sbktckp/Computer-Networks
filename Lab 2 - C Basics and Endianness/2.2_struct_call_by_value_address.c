/*
 * 2.2 Struct, Nested Struct, Call-by-Value vs Call-by-Address
 *
 * Aim: populate struct student_info (embeds struct dob), pass it to one
 *      function by value and another by address, print members from
 *      inside each to show which mutation survives the call.
 *
 * Design notes:
 *   - modify_by_value() receives a full copy of the struct on its own
 *     stack frame. Every write inside it dies when the function
 *     returns; main's copy is untouched. modify_by_address() receives
 *     a pointer, so its writes land in main's actual memory.
 *   - strncpy() with an explicit guaranteed null terminator over raw
 *     strcpy(): strcpy() will happily overflow name[50] if the source
 *     is longer; strncpy() caps the copy, and since strncpy() itself
 *     does not guarantee termination when the source is >= the limit,
 *     the terminator is set explicitly.
 *
 * Run:    ./2.2_struct_call_by_value_address
 * Output: Original          : roll=101 name=Aditi Sharma   cgpa=8.75 dob=14/03/2005
 *         Inside by_value   : roll=999 name=CHANGED        cgpa=0.00 dob=01/01/1999
 *         Inside by_address : roll=101 name=Aditi Sharma   cgpa=9.10 dob=15/03/2005
 *         After calls       : roll=101 name=Aditi Sharma   cgpa=9.10 dob=15/03/2005
 */
#include <stdio.h>
#include <string.h>

struct dob {
    int day, month, year;
};

struct student_info {
    int roll_no;
    char name[50];
    float cgpa;
    struct dob dob;
};

static void set_name(struct student_info *s, const char *name) {
    strncpy(s->name, name, sizeof(s->name) - 1);
    s->name[sizeof(s->name) - 1] = '\0';
}

static void print_student(const char *label, const struct student_info *s) {
    printf("%-18s: roll=%d name=%-14s cgpa=%.2f dob=%02d/%02d/%04d\n",
           label, s->roll_no, s->name, s->cgpa,
           s->dob.day, s->dob.month, s->dob.year);
}

/* Whole struct copied onto this frame; nothing written here outlives the call. */
static void modify_by_value(struct student_info s) {
    s.roll_no = 999;
    set_name(&s, "CHANGED");
    s.cgpa = 0.0f;
    s.dob = (struct dob){1, 1, 1999};
    print_student("Inside by_value", &s);
}

/* Address crosses the call, so writes go through to the caller's struct. */
static void modify_by_address(struct student_info *s) {
    s->cgpa = 9.10f;
    s->dob.day = 15;
    print_student("Inside by_address", s);
}

int main(void) {
    struct student_info stu = {
        .roll_no = 101,
        .cgpa = 8.75f,
        .dob = {14, 3, 2005}
    };
    set_name(&stu, "Aditi Sharma");

    print_student("Original", &stu);
    modify_by_value(stu);        /* stu in main is unaffected */
    modify_by_address(&stu);     /* stu in main is now updated */
    print_student("After calls", &stu);

    return 0;
}
