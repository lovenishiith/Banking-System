#include "dashboard.h"
#include "db.h"
#include <iostream>
using namespace std;

void showDashboard(int user_id , string name){

    cout << "\n===== DASHBOARD =====\n";
    cout << "Welcome " << name << endl;

    try {
        sql::Connection *con = getConnection();

        // 🔹 Show Balance
        sql::PreparedStatement *balStmt = con->prepareStatement(
            "SELECT balance FROM accounts WHERE user_id = ?"
        );

        balStmt->setInt(1, user_id);
        sql::ResultSet *balRes = balStmt->executeQuery();

        if(balRes->next()){
    cout << "Balance: Rs" << balRes->getDouble("balance") << endl;
} else {
    cout << "No account found.\n";
}

        delete balRes;
        delete balStmt;

        // 🔹 Last 3 transactions
        cout << "\nRecent Transactions:\n";

        sql::PreparedStatement *txnStmt = con->prepareStatement(
            "SELECT t.*, s.name AS sender_name, r.name AS receiver_name "
            "FROM transactions t "
            "LEFT JOIN users s ON t.sender_id = s.id "
            "LEFT JOIN users r ON t.receiver_id = r.id "
            "WHERE t.sender_id = ? OR t.receiver_id = ? "
            "ORDER BY created_at DESC LIMIT 3"
        );

        txnStmt->setInt(1, user_id);
        txnStmt->setInt(2, user_id);

sql::ResultSet *txnRes = txnStmt->executeQuery();

bool found = false;

while (txnRes->next()) {
    found = true;

    string sender = txnRes->isNull("sender_name") ? "SYSTEM" : txnRes->getString("sender_name");
    string receiver = txnRes->isNull("receiver_name") ? "SYSTEM" : txnRes->getString("receiver_name");

    string type = txnRes->getString("type");
    double amount = txnRes->getDouble("amount");

    int sender_id_db = txnRes->isNull("sender_id") ? -1 : txnRes->getInt("sender_id");

    string sign = "+";

    if(type == "DEPOSIT") sign = "+";
    else if(type == "WITHDRAW") sign = "-";
    else if(type == "TRANSFER"){
        sign = (sender_id_db == user_id) ? "-" : "+";
    }

    cout << sender << " → " << receiver
         << " | " << type
         << " | " << sign << "Rs" << amount
         << endl;
}

if(!found){
    cout << "No transactions yet.\n";
}
        delete txnRes;
        delete txnStmt;
        delete con;

    }
    catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}