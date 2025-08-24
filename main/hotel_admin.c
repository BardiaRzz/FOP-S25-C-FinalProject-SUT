#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseConsole()
{
    printf("\nPress Enter to continue...");
    getchar();
}

void changeAdminPassword() {
    char currentPass[100], storedUsername[100], storedPass[100], newPass[100];
    FILE *file = fopen("admin_pass.txt", "r");

    if (!file) {
        printf("Error: Could not open admin_pass.txt\n");
        return;
    }

    // Read both username and password from file
    fscanf(file, "%[^,],%s", storedUsername, storedPass);
    fclose(file);

    printf("Enter current password: ");
    scanf(" %[^\n]", currentPass);

    if (strcmp(currentPass, storedPass) != 0) {
        printf("Incorrect password. Aborting.\n");
        return;
    }

    printf("Enter new password: ");
    scanf(" %[^\n]", newPass);

    file = fopen("admin_pass.txt", "w");
    if (!file) {
        printf("Error: Could not write to admin_pass.txt\n");
        return;
    }

    // Write the username and new password
    fprintf(file, "%s,%s", storedUsername, newPass);
    fclose(file);

    printf("Password changed successfully.\n");
}

void viewAllReservations()
{
    clearScreen();
    FILE *file = fopen("booking.txt", "r");
    if (!file)
    {
        printf("Error: booking.txt not found.\n");
        return;
    }
    char room[20], name[50], phone[15], date[20];
    printf("All Reservations:\n------------------\n");
    while (fscanf(file, "%19[^,],%49[^,],%14[^,],%19s\n", name, phone, date, room) == 4)
    {
        printf("Name: %s | Phone: %s | Date: %s | Room: %s\n", name, phone, date, room);
    }
    fclose(file);
}

void searchReservations()
{
    clearScreen();
    int filter;
    char input[50];
    printf("Search Reservations By:\n1. Date\n2. Name\n3. Phone\nSelect: ");
    scanf("%d", &filter);
    getchar();

    printf("Enter value to search: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    FILE *file = fopen("booking.txt", "r");
    if (!file)
    {
        printf("Error: booking.txt not found.\n");
        return;
    }

    char room[20], name[50], phone[15], date[20];
    int found = 0;
    while (fscanf(file, "%19[^,],%49[^,],%14[^,],%19s\n", name, phone, room, date) == 4)
    {
        if ((filter == 1 && strcmp(date, input) == 0) ||
            (filter == 2 && strcmp(name, input) == 0) ||
            (filter == 3 && strcmp(phone, input) == 0))
        {
            printf("Name: %s | Phone: %s | Date: %s | Room: %s\n", name, phone, room, date);
            found = 1;
        }
    }
    if (!found)
        printf("No reservation found.\n");
    fclose(file);
}

void defineRooms()
{
    clearScreen();
    char room[20];
    printf("Enter room name to add (e.g., room101): ");
    fgets(room, sizeof(room), stdin);
    room[strcspn(room, "\n")] = '\0';

    FILE *file = fopen("rooms.txt", "a");
    if (!file)
    {
        printf("Error opening rooms.txt\n");
        return;
    }
    fprintf(file, "%s\n", room);
    fclose(file);
    printf("Room added successfully.\n");
}

void setRoomPrices()
{
    clearScreen();
    printf("Room Price List:\n-----------------\n");

    FILE *roomFile = fopen("rooms.txt", "r");
    FILE *priceFile = fopen("prices.txt", "r");

    if (!roomFile)
    {
        printf("Error: rooms.txt not found.\n");
        if (priceFile)
            fclose(priceFile);
        return;
    }

    typedef struct
    {
        char room[20];
        int price;
    } RoomPrice;
    RoomPrice prices[100];
    int priceCount = 0;

    if (priceFile)
    {
        while (fscanf(priceFile, "%19[^,],%d\n", prices[priceCount].room, &prices[priceCount].price) == 2)
        {
            priceCount++;
        }
        fclose(priceFile);
    }

    char room[20];
    while (fgets(room, sizeof(room), roomFile))
    {
        room[strcspn(room, "\n")] = '\0';
        int found = 0;
        for (int i = 0; i < priceCount; i++)
        {
            if (strcmp(prices[i].room, room) == 0)
            {
                printf("%s: %d\n", room, prices[i].price);
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("%s: 0\n", room);
        }
    }
    fclose(roomFile);

    char newRoom[20];
    int newPrice;
    printf("\nEnter room name to set/update price: ");
    fgets(newRoom, sizeof(newRoom), stdin);
    newRoom[strcspn(newRoom, "\n")] = '\0';

    printf("Enter price: ");
    scanf("%d", &newPrice);
    getchar();

    // Rewrite prices
    FILE *writeFile = fopen("prices.txt", "w");
    if (!writeFile)
    {
        printf("Error opening prices.txt\n");
        return;
    }
    int updated = 0;
    for (int i = 0; i < priceCount; i++)
    {
        if (strcmp(prices[i].room, newRoom) == 0)
        {
            fprintf(writeFile, "%s,%d\n", newRoom, newPrice);
            updated = 1;
        }
        else
        {
            fprintf(writeFile, "%s,%d\n", prices[i].room, prices[i].price);
        }
    }
    if (!updated)
    {
        fprintf(writeFile, "%s,%d\n", newRoom, newPrice);
    }
    fclose(writeFile);
    printf("Room price updated.\n");
}

void viewUsers()
{
    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        printf("users.txt not found.\n");
        return;
    }
    char name[50], phone[15];
    printf("Users:\n------\n");
    while (fscanf(file, "%49[^,],%14s\n", name, phone) == 2)
    {
        printf("%s - %s\n", name, phone);
    }
    fclose(file);
}

void addUser()
{
    char name[50], phone[15];
    printf("Enter new user name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter phone: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';

    FILE *file = fopen("users.txt", "a");
    if (!file)
    {
        printf("Error opening users.txt\n");
        return;
    }
    fprintf(file, "%s,%s\n", name, phone);
    fclose(file);
    printf("User added.\n");
}

void removeUser()
{
    char target[15];
    printf("Enter phone of user to remove: ");
    fgets(target, sizeof(target), stdin);
    target[strcspn(target, "\n")] = '\0';

    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp)
    {
        printf("Error opening files.\n");
        return;
    }

    char name[50], phone[15];
    int found = 0;
    while (fscanf(file, "%49[^,],%14s\n", name, phone) == 2)
    {
        if (strcmp(phone, target) != 0)
            fprintf(temp, "%s,%s\n", name, phone);
        else
            found = 1;
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User removed.\n");
    else
        printf("User not found.\n");
}

void manageUsers()
{
    int choice;
    do
    {
        clearScreen();
        printf("Manage Users\n------------\n");
        printf("1. View Users\n2. Add User\n3. Remove User\n0. Back\nSelect: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            viewUsers();
            break;
        case 2:
            addUser();
            break;
        case 3:
            removeUser();
            break;
       
        case 0:
            break; default:
            printf("Invalid choice\n");
        }
        if (choice != 0)
            pauseConsole();
    } while (choice != 0);
}

void adminMenu()
{
    int option;
    do
    {
        clearScreen();
        printf("=== Admin Panel ===\n");
        printf("1. View Reservations\n");
        printf("2. Search Reservations\n");
        printf("3. Define Rooms\n");
        printf("4. Set Room Prices\n");
        printf("5. Manage Users\n");
        printf("6. Change Admin Password\n");
        printf("0. Logout\n");
        printf("Select option: ");
        scanf("%d", &option);
        getchar();

        switch (option)
        {
        case 1:
            viewAllReservations();
            break;
        case 2:
            searchReservations();
            break;
        case 3:
            defineRooms();
            break;
        case 4:
            setRoomPrices();
            break;
        case 5:
            manageUsers();
            break;
        case 6:
            changeAdminPassword();
            break;

        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
        }
        if (option != 0)
            pauseConsole();
    } while (option != 0);
}
