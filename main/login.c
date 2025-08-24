#include <stdio.h>
#include <string.h>
#include "login.h"
#include "hotel_admin.h"
#include "hotel_user.h"


void userLogin() {
    char name[100];
    char phone[20];
    int found = 0;

    FILE *file = fopen("users.txt", "a+");
    if (!file) {
        printf("Error opening users.txt\n");
        return;
    }

    printf("=== User Login / Register ===\n");
    printf("Enter your name: ");
    scanf(" %[^\n]", name);
    printf("Enter your phone number: ");
    scanf(" %[^\n]", phone);

    rewind(file);

    char fileName[100], filePhone[20];
    while (fscanf(file, "%[^,],%[^\n]\n", fileName, filePhone) != EOF) {
        if (strcmp(name, fileName) == 0 && strcmp(phone, filePhone) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(file, "%s,%s\n", name, phone);
        printf("User not found. Registered new user.\n");
    } else {
        printf("Login successful.\n");
    }

    fclose(file);
    User user;
    strcpy(user.name, name);
    strcpy(user.phone, phone);
    userMenu(user);

}

int adminLogin(const char* username, const char* password) {
    FILE* file = fopen("admin_pass.txt", "r");
    if (!file) {
        perror("Could not open admin_pass.txt");
        return 0;
    }

    char fileUser[50], filePass[50];
    while (fscanf(file, "%49[^,],%49s\n", fileUser, filePass) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
