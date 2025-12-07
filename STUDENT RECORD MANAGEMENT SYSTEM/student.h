#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_ID 15
#define MAX_BRANCH 10
#define FILE_NAME "students.dat"

typedef struct {
    char id[MAX_ID];
    char name[MAX_NAME];
    float cgpa;
    int yearOfStudy;
    char branch[MAX_BRANCH];
    int semester;
} Student;

// Function Prototypes
void addStudent();
void displayStudents();
void searchStudent();
void modifyStudent();
void deleteStudent();
void sortStudents();

#endif
