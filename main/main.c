#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "hotel_admin.h"
#include "hotel_user.h"

int main()
{
    while (1)
    {
        int choice;
        clearScreen();
        printf("=== Hotel Reservation System ===\n");
        printf("1. Login as User\n");
        printf("2. Login as Admin\n");
        printf("0. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            userLogin();
            printf("User login successful!\n");
        }
        else if (choice == 2)
        {
            Admin admin;
            int success = 0;

            while (!success)
            {
                printf("Admin Login\n-----------\n(Enter 0 as username to cancel)\n");

                printf("Enter username: ");
                fgets(admin.username, sizeof(admin.username), stdin);
                admin.username[strcspn(admin.username, "\n")] = '\0';

                if (strcmp(admin.username, "0") == 0)
                    break;

                printf("Enter password: ");
                fgets(admin.password, sizeof(admin.password), stdin);
                admin.password[strcspn(admin.password, "\n")] = '\0';

                if (adminLogin(admin.username, admin.password))
                {
                    printf("Admin login successful!\n");
                    success = 1;
                    adminMenu();
                }
                else
                {
                    printf("Login failed. Please try again.\n\n");
                }
            }
        }
        else if (choice == 0)
        {
            printf("Exiting... Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    pauseConsole();
    return 0;
}
