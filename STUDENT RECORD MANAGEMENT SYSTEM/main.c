#include "student.h"

int main() {
    int choice;
    while(1) {
        printf("\n=== Student Record Management System ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Modify Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            // Handle non-integer input to prevent infinite loop
            while(getchar() != '\n'); // Clear buffer
            choice = -1; // Force default case
        }

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: modifyStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 0: 
                printf("Exiting...\n");
                exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
