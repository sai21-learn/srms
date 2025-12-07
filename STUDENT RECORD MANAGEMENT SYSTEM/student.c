#include "student.h"

// Helper function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addStudent() {
    Student s;
    FILE *fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Add Student ---\n");

    // Year of Study
    do {
        printf("Enter Year of Study (1-4): ");
        if (scanf("%d", &s.yearOfStudy) != 1) {
            clearInputBuffer();
            s.yearOfStudy = 0;
        }
    } while (s.yearOfStudy < 1 || s.yearOfStudy > 4);
    clearInputBuffer();

    // Branch
    printf("Enter Branch (e.g., CSE, ECE): ");
    fgets(s.branch, MAX_BRANCH, stdin);
    s.branch[strcspn(s.branch, "\n")] = 0;

    // Semester
    printf("Enter Semester: ");
    scanf("%d", &s.semester);
    clearInputBuffer();

    // ID Validation Logic logic
    // Mapping: 4th -> AP22, 3rd -> AP23, 2nd -> AP24, 1st -> AP25
    char expectedPrefix[5];
    if (s.yearOfStudy == 4) strcpy(expectedPrefix, "AP22");
    else if (s.yearOfStudy == 3) strcpy(expectedPrefix, "AP23");
    else if (s.yearOfStudy == 2) strcpy(expectedPrefix, "AP24");
    else strcpy(expectedPrefix, "AP25");

    while (1) {
        printf("Enter ID (Must start with %s): ", expectedPrefix);
        fgets(s.id, MAX_ID, stdin);
        s.id[strcspn(s.id, "\n")] = 0;
        
        if (strncmp(s.id, expectedPrefix, 4) == 0) {
            // Check for duplicate ID
            int duplicate = 0;
            Student temp;
            FILE *checkFp = fopen(FILE_NAME, "rb");
            if (checkFp) {
                while (fread(&temp, sizeof(Student), 1, checkFp)) {
                    if (strcmp(temp.id, s.id) == 0) {
                        duplicate = 1;
                        break;
                    }
                }
                fclose(checkFp);
            }

            if (duplicate) {
                printf("Error: Student with ID %s already exists!\n", s.id);
            } else {
                break;
            }
        } else {
            printf("Invalid ID format! For Year %d, ID must start with %s.\n", s.yearOfStudy, expectedPrefix);
        }
    }

    printf("Enter Name: ");
    fgets(s.name, MAX_NAME, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter CGPA: ");
    scanf("%f", &s.cgpa);
    clearInputBuffer();

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully!\n");
}

void displayStudents() {
    Student s;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n%-15s %-20s %-8s %-5s %-5s %-5s\n", "ID", "Name", "Branch", "Year", "Sem", "CGPA");
    printf("------------------------------------------------------------------\n");
    
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%-15s %-20s %-8s %-5d %-5d %-5.2f\n", s.id, s.name, s.branch, s.yearOfStudy, s.semester, s.cgpa);
    }
    fclose(fp);
}

void searchStudent() {
    Student s;
    char searchId[MAX_ID];
    int found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\nEnter ID to search: ");
    fgets(searchId, MAX_ID, stdin);
    searchId[strcspn(searchId, "\n")] = 0;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, searchId) == 0) {
            printf("\nRecord Found:\n");
            printf("ID: %s\n", s.id);
            printf("Name: %s\n", s.name);
            printf("Branch: %s\n", s.branch);
            printf("Year: %d\n", s.yearOfStudy);
            printf("Semester: %d\n", s.semester);
            printf("CGPA: %.2f\n", s.cgpa);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Student with ID %s not found.\n", searchId);
    }
    fclose(fp);
}

void modifyStudent() {
    Student s;
    char searchId[MAX_ID];
    int found = 0;
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\nEnter ID to modify: ");
    fgets(searchId, MAX_ID, stdin);
    searchId[strcspn(searchId, "\n")] = 0;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, searchId) == 0) {
            printf("Modify Record for %s (Press Enter to keep current value):\n", s.name);
            
            char buffer[MAX_NAME];

            printf("Enter New Name [%s]: ", s.name);
            fgets(buffer, MAX_NAME, stdin);
            if (buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(s.name, buffer);
            }

            // For Branch/Year/Sem, simplify to just Name/CGPA/Branch for now or full edit? 
            // The prompt specifically asked for Branch/Sem prompts at add, implies they are important.
            // Let's allow modifying basic details.

            printf("Enter New Branch [%s]: ", s.branch);
            fgets(buffer, MAX_BRANCH, stdin);
            if (buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(s.branch, buffer);
            }

            printf("Enter New Semester [%d]: ", s.semester);
            fgets(buffer, MAX_NAME, stdin);
            if (buffer[0] != '\n') {
                s.semester = atoi(buffer);
            }

            printf("Enter New CGPA [%.2f]: ", s.cgpa);
            fgets(buffer, MAX_NAME, stdin);
            if (buffer[0] != '\n') {
                s.cgpa = atof(buffer);
            }
            // Not allowing Year/ID change to avoid validation complexity here

            fseek(fp, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);
            found = 1;
            printf("Record updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Student with ID %s not found.\n", searchId);
    }
    fclose(fp);
}

void deleteStudent() {
    Student s;
    char searchId[MAX_ID];
    int found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\nEnter ID to delete: ");
    fgets(searchId, MAX_ID, stdin);
    searchId[strcspn(searchId, "\n")] = 0;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, searchId) == 0) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found) {
        printf("Record deleted successfully!\n");
    } else {
        printf("Student with ID %s not found.\n", searchId);
    }
}

// Comparison function for qsort
int compareStudents(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    return strcmp(s1->id, s2->id);
}

void sortStudents() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    // Count records
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);
    int count = fileSize / sizeof(Student);

    if (count == 0) {
        fclose(fp);
        printf("No records to sort.\n");
        return;
    }

    Student *students = (Student *)malloc(count * sizeof(Student));
    fread(students, sizeof(Student), count, fp);
    fclose(fp);

    // Sort
    qsort(students, count, sizeof(Student), compareStudents);

    // Write back
    fp = fopen(FILE_NAME, "wb");
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
    free(students);

    printf("Students sorted by ID successfully!\n");
}
