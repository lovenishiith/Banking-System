#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct UserSession {
    int id;
    string name;
    string email;
    string role;
};

UserSession loginUser();
void createUser();

void freezeUser();
void unfreezeUser();
void viewAllUsers();
void viewBlockedUsers();
void unblockUser();

void viewSuspiciousTransactions();

#endif