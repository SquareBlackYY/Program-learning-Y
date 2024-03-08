#include <stdio.h>

typedef struct
{
    int year;
    int month;
    int day;
} Birthday;

typedef struct
{
    int id;
    char *name;
    double score;
    Birthday birthday;
} Student;

void printstudentinfo(Student *pStu)
{
    printf("ID:%d\tName:%s\tScore:%f\tBirthday:%d %d %d\n", pStu->id, pStu->name, pStu->score,
    pStu->birthday.year, pStu->birthday.month, pStu->birthday.day);
}

int main()
{
    Student stu1 = {1001, "yy", 98, {2003, 2, 4}};
    Student *p1 = &stu1;
    printstudentinfo(p1);
    return 0;
}