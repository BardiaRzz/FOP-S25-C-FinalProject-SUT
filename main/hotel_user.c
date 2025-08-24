#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hotel_admin.h"

#define MAX_ROOMS 100
#define MAX_LINE 256

typedef struct {
    char name[50];
    char phone[20];
} User;

typedef struct {
    char room[10];
    int price;
} RoomPrice;


void viewUserReservations(User user) {
    FILE *file = fopen("booking.txt", "r");
    if (!file) {
        printf("No reservations found.\n");
        return;
    }

    char line[MAX_LINE];
    int found = 0;
    printf("\n--- Your Reservations ---\n");
    while (fgets(line, sizeof(line), file)) {
        char name[50], phone[20], room[10], date[20];
        sscanf(line, "%[^,],%[^,],%[^,],%s", name, phone, room, date);
        if (strcmp(name, user.name) == 0 && strcmp(phone, user.phone) == 0) {
            printf("Room: %s | Date: %s\n", room, date);
            found = 1;
        }
    }

    if (!found) {
        printf("No reservations found for you.\n");
    }

    fclose(file);
}

int isRoomReserved(const char *room, const char *date) {
    FILE *file = fopen("booking.txt", "r");
    if (!file) return 0;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        char name[50], phone[20], r[10], d[20];
        sscanf(line, "%[^,],%[^,],%[^,],%s", name, phone, r, d);
        if (strcmp(r, room) == 0 && strcmp(d, date) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int getRoomPrice(const char *room) {
    FILE *file = fopen("prices.txt", "r");
    if (!file) return -1;

    char line[MAX_LINE], r[10];
    int price;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%d", r, &price);
        if (strcmp(r, room) == 0) {
            fclose(file);
            return price;
        }
    }

    fclose(file);
    return -1;
}

int showAvailableRooms(char availableRooms[][10], int roomPrices[], const char *date) {
    FILE *file = fopen("rooms.txt", "r");
    if (!file) {
        printf("Error: Could not open rooms.txt\n");
        return 0;
    }

    int count = 0;
    char room[10];
    while (fgets(room, sizeof(room), file)) {
        room[strcspn(room, "\n")] = 0; 
        if (!isRoomReserved(room, date)) {
            int price = getRoomPrice(room);
            if (price != -1) {
                strcpy(availableRooms[count], room);
                roomPrices[count] = price;
                count++;
            }
        }
    }

    fclose(file);
    return count;
}

void makeReservation(User user) {
    char date[20];
    char availableRooms[MAX_ROOMS][10];
    int roomPrices[MAX_ROOMS];
    int roomCount;

    do {
        clearScreen();
        printf("--- Make a Reservation ---\n");
        printf("Enter desired date (YYYY-MM-DD) 0 for exit: ");
        scanf("%s", date);
        getchar();

        if (strcmp(date, "0") == 0) {
            printf("Reservation cancelled.\n");
            return;
        }

        roomCount = showAvailableRooms(availableRooms, roomPrices, date);

        if (roomCount == 0) {
            printf("No available rooms on %s.\nPlease select another date.\n", date);
            pauseConsole();
        }
    } while (roomCount == 0);

    printf("\nAvailable Rooms:\n");
    for (int i = 0; i < roomCount; i++) {
        printf("%d. Room %s - Price: %d Toman\n", i + 1, availableRooms[i], roomPrices[i]);
    }

    int choice;
    printf("Select a room (enter number): ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > roomCount) {
        printf("Invalid room selection.\n");
        return;
    }

    char *selectedRoom = availableRooms[choice - 1];
    FILE *file = fopen("booking.txt", "a");
    if (!file) {
        printf("Error: Cannot write to booking.txt\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%s\n", user.name, user.phone, selectedRoom, date);
    fclose(file);
    printf("Reservation successful for room %s on %s.\n", selectedRoom, date);
}

void userMenu(User user) {
    int choice;
    do {
        clearScreen();
        printf("Welcome, %s!\n", user.name);
        printf("\n--- User Menu ---\n");
        printf("1. View My Reservations\n");
        printf("2. Make New Reservation\n");
        printf("0. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                clearScreen();
                viewUserReservations(user);
                pauseConsole();
                break;
            case 2:
                makeReservation(user);
                pauseConsole();
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                pauseConsole();
        }
    } while (choice != 0);
}
