#ifndef LOGIN_H
#define LOGIN_H

typedef struct {
    char name[50];
    char phone[15];
} User;

typedef struct {
    char username[50];
    char password[50];
} Admin;

void userLogin();
int adminLogin(const char* username, const char* password);

#endif
