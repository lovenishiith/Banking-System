// con -> connection
//Prepared statement -> means sql template for placeholders
// Result set -> table returned by select query
// commit -> means save changes permanently 
// con->setAutoCommit(false) -> means dont save automatically , i will decide when to save 

#include<iostream>
#include <mysql/jdbc.h>
#include <iomanip> 
#include<fstream>
#include<ctime>
#include<cstdlib>
using namespace std;
#include <functional>
#include "user.h"
#include "transaction.h"
#include "utils.h"
#include "dashboard.h"
void showMenu(){
    cout << "\n====== BANKING SYSTEM ======\n";
    cout << "1. Create User\n";
    cout << "2. Login\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer\n";
    cout << "6. View Transactions\n";
    cout << "7. Check Balance\n";
    cout << "8. Logout\n";
    cout << "9. View Fraud Transactions\n";
    cout << "10.Admin Panel\n";
    cout << "11.Account Summary\n";
    cout << "12. Monthly Spending Analytics\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";
}
int main(){
    int choice;
    UserSession current_user = {-1, "", "",""};

    while(true){
        if(current_user.id != -1){
            cout << "\nLogged in as : " << current_user.name << endl;
        }

        showMenu();
        if(!(cin >> choice)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    continue;
}

        switch(choice){

            case 1: 
                cout << "Create user selected\n";
                createUser();
                break;

            case 2:
                cout << "Login User Selected\n";
                current_user = loginUser();
                if(current_user.id != -1){
                    showDashboard(current_user.id, current_user.name);
                }
                break;

            case 3: 
                cout << "Deposit selected\n";
                if (current_user.id == -1 ) {
                    cout << "Please login first\n";
                } else {
                    deposit(current_user.id);
                    showDashboard(current_user.id, current_user.name);
                }
                break;

            case 4: 
                cout << "Withdraw selected\n";
                if(current_user.id == -1){
                    cout << "Please Login First\n";
                } else {
                    withdraw(current_user.id);
                    showDashboard(current_user.id, current_user.name);
                }
                break;    

            case 5: 
                cout << "Transfer selected\n";
                if(current_user.id == -1){
                    cout << "Please Login First\n";
                } else {
                    transfer(current_user.id);
                    showDashboard(current_user.id, current_user.name);
                }
                break;

            case 6: 
                cout << "View Transactions selected\n";
                if (current_user.id == -1) {
                    cout << "Please login first\n";
                } else {
                    viewTransactions(current_user.id);
                }
                break;
            case 7 : 
                cout << "Check Balance selected\n";
                if(current_user.id == -1){
                    cout << "Please login first\n";
                }  else {
                    CheckBalance(current_user.id);
                   
                }
                 break;
            case 8:
                cout << "Logout Selected\n";
                if(current_user.id == -1){
                    cout << "No user is currently logged in\n";
                } else {
                    current_user = {-1,"","",""};
                    cout << "Logged out successfully\n";
                }
                break;
            
            case 9:
                cout << "Viewing Fraud Transactions\n";
                viewSuspiciousTransactions();
                break;
            
            case 10:
                if(current_user.id != -1 && current_user.role == "ADMIN"){
                    int adminChoice;
                    cout << "\n===== ADMIN PANEL =====\n";
                    cout << "1.View All Users\n";
                    cout << "2.View Blocked Users\n";
                    cout << "3.Unblock User\n";
                    cout << "4.Freeze User\n";
                    cout << "5.Unfreeze User\n";
                    cout << "Enter Choice: ";
                    if(!(cin >> adminChoice)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    break;
}

                    if(adminChoice == 1) viewAllUsers();
                    else if(adminChoice == 2) viewBlockedUsers();
                    else if(adminChoice == 3) unblockUser();
                    else if(adminChoice == 4) freezeUser();
                    else if(adminChoice == 5) unfreezeUser();
                    else cout << "Invalid choice\n";
                } 
                else {
                    cout << "Access denied\n";
                }
                break;
            case 11:
                if(current_user.id == -1){
                cout << "Please login first\n";
                } else {
                showAccountSummary(current_user.id);
                }
                break; 
            case 12:
                if(current_user.id == -1){
                    cout << "Please login first\n";
                } else {
                    showMonthlySpending(current_user.id);
                }
                break;
            case 0: 
                cout << "Exiting....\n";
                return 0;

            default: 
                cout << "Invalid choice\n";            
        }
    }
}