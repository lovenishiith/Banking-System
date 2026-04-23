#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

// Transaction operations
void deposit(int user_id, double amount, string note);
void withdraw(int user_id, double amount, string note);
void transfer(int sender_id);

// Viewing data
void viewTransactions(int user_id);
void CheckBalance(int user_id);

// Analytics
void showAccountSummary(int user_id);
void showMonthlySpending(int user_id);

#endif
