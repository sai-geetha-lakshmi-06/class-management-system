#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "classes.dat"
#define MAX_LEN 100

// ------------------ STRUCT DEFINITION ------------------
typedef struct {
    int classId;
    char className[MAX_LEN];
    char classTeacher[MAX_LEN];
    int totalStudents;
    char section[MAX_LEN];
} Class;

// ------------------ FUNCTION DECLARATIONS ------------------
void addClass();
void viewClasses();
void searchClass();
void updateClass();
void deleteClass();
int classIdExists(int id);
void saveClass(Class c);
void loadClasses(Class classes[], int *count);
void writeAllClasses(Class classes[], int count);

// ------------------ MAIN MENU ------------------
int main() {
    int choice;
    do {
        printf("\n==============================\n");
        printf("      CLASS MANAGEMENT\n");
        printf("==============================\n");
        printf("1. Add New Class\n");
        printf("2. View All Classes\n");
        printf("3. Search Class\n");
        printf("4. Update Class\n");
        printf("5. Delete Class\n");
        printf("6. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addClass(); break;
            case 2: viewClasses(); break;
            case 3: searchClass(); break;
            case 4: updateClass(); break;
            case 5: deleteClass(); break;
            case 6: printf("Exiting program.\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);

    return 0;
}

// ------------------ ADD CLASS ------------------
void addClass() {
    Class c;

    printf("\n--- Add New Class ---\n");
    printf("Enter Class ID: ");
    scanf("%d", &c.classId);
    getchar();

    if (classIdExists(c.classId)) {
        printf("Error: Class ID already exists!\n");
        return;
    }

    printf("Enter Class Name: ");
    fgets(c.className, sizeof(c.className), stdin);
    c.className[strcspn(c.className, "\n")] = '\0';

    printf("Enter Class Teacher: ");
    fgets(c.classTeacher, sizeof(c.classTeacher), stdin);
    c.classTeacher[strcspn(c.classTeacher, "\n")] = '\0';

    printf("Enter Section: ");
    fgets(c.section, sizeof(c.section), stdin);
    c.section[strcspn(c.section, "\n")] = '\0';

    printf("Enter Total Students: ");
    scanf("%d", &c.totalStudents);

    saveClass(c);
    printf("Class added successfully!\n");
}

// ------------------ SAVE CLASS TO FILE ------------------
void saveClass(Class c) {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    fwrite(&c, sizeof(Class), 1, fp);
    fclose(fp);
}

// ------------------ CHECK IF CLASS ID EXISTS ------------------
int classIdExists(int id) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;

    Class c;
    while (fread(&c, sizeof(Class), 1, fp)) {
        if (c.classId == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ------------------ VIEW ALL CLASSES ------------------
void viewClasses() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No class records found.\n");
        return;
    }

    Class c;
    printf("\n%-10s %-20s %-20s %-10s %-10s\n",
    "ID", "Class Name", "Teacher", "Section", "Students");
    printf("-----------------------------------------------------------------\n");

    while (fread(&c, sizeof(Class), 1, fp)) {
        printf("%-10d %-20s %-20s %-10s %-10d\n",
    c.classId, c.className, c.classTeacher, c.section, c.totalStudents);
    }
    fclose(fp);
}

// ------------------ SEARCH CLASS ------------------
void searchClass() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    Class c;

    printf("\nEnter Class ID to search: ");
    scanf("%d", &id);

    while (fread(&c, sizeof(Class), 1, fp)) {
        if (c.classId == id) {
            printf("\nClass Found:\n");
            printf("ID: %d\nName: %s\nTeacher: %s\nSection: %s\nTotal Students: %d\n",
            c.classId, c.className, c.classTeacher, c.section, c.totalStudents);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No class found with ID %d.\n", id);

    fclose(fp);
}

// ------------------ UPDATE CLASS ------------------
void updateClass() {
    Class classes[100];
    int count = 0, id, found = 0;

    loadClasses(classes, &count);

    printf("\nEnter Class ID to update: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (classes[i].classId == id) {
            found = 1;

            printf("Enter new Class Teacher: ");
            fgets(classes[i].classTeacher, sizeof(classes[i].classTeacher), stdin);
            classes[i].classTeacher[strcspn(classes[i].classTeacher, "\n")] = '\0';

            printf("Enter new Section: ");
            fgets(classes[i].section, sizeof(classes[i].section), stdin);
            classes[i].section[strcspn(classes[i].section, "\n")] = '\0';

            printf("Enter new Total Students: ");
            scanf("%d", &classes[i].totalStudents);

            break;
        }
    }

    if (found) {
        writeAllClasses(classes, count);
        printf("Record updated successfully.\n");
    } else {
        printf("Class ID not found.\n");
    }
}

// ------------------ DELETE CLASS ------------------
void deleteClass() {
    Class classes[100];
    int count = 0, id, found = 0;

    loadClasses(classes, &count);

    printf("\nEnter Class ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (classes[i].classId == id) {
            found = 1;

            printf("Are you sure you want to delete %s (ID %d)? (y/n): ",
                classes[i].className, id);

            char confirm = getchar();
            getchar();

            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < count - 1; j++)
                    classes[j] = classes[j + 1];
                count--;

                writeAllClasses(classes, count);
                printf("Record deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }

            break;
        }
    }

    if (!found)
        printf("Class ID not found.\n");
}

// ------------------ LOAD & WRITE ALL CLASSES ------------------
void loadClasses(Class classes[], int *count) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return;
    *count = 0;
    while (fread(&classes[*count], sizeof(Class), 1, fp))
        (*count)++;
    fclose(fp);
}

void writeAllClasses(Class classes[], int count) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (!fp) {
        perror("Error writing file");
        return;
    }
    fwrite(classes, sizeof(Class), count, fp);
    fclose(fp);
}


