#ifndef TRANSACTION_H
#define TRANSACTION_H
// Transaction operations
void deposit(int user_id);
void withdraw(int user_id);
void transfer(int user_id);

// Viewing data
void viewTransactions(int user_id);
void CheckBalance(int user_id);

// Analytics
void showAccountSummary(int user_id);
void showMonthlySpending(int user_id);
#endif