#include "transaction.h"
#include "db.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;
void deposit(int user_id){

    double amount;
    cout << "Enter Amount to Deposit: ";
    if(!(cin >> amount)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid amount\n";
    return; // ✅ exit function
}

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ✅ FIX

    string note;
    cout << "Enter note/description: ";
    getline(cin, note);

    if(amount <= 0){
        cout << "Invalid amount!" << endl;
        return;
    }

    sql::Connection *con = nullptr;

    try {
        con = getConnection();

        sql::PreparedStatement *checkStmt = con->prepareStatement(
            "SELECT 1 FROM accounts WHERE user_id = ?"
        );
        checkStmt->setInt(1, user_id);

        sql::ResultSet *res = checkStmt->executeQuery();

        if(!res->next()){
            cout << "User not found!" << endl;
            delete res; delete checkStmt; delete con;
            return;
        }

        delete res;
        delete checkStmt;

        con->setAutoCommit(false);

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "UPDATE accounts SET balance = balance + ? WHERE user_id = ?"
        );

        pstmt->setDouble(1, amount);
        pstmt->setInt(2, user_id);
        pstmt->executeUpdate();

        bool isFraud = (amount > 50000);

        sql::PreparedStatement *log = con->prepareStatement(
            "INSERT INTO transactions (sender_id, receiver_id, type, amount, is_suspicious, description) VALUES (?, ?, 'DEPOSIT', ?, ?, ?)"
        );

        log->setNull(1, sql::DataType::INTEGER);
        log->setInt(2, user_id);
        log->setDouble(3, amount);
        log->setBoolean(4, isFraud);
        log->setString(5, note);

        log->executeUpdate();

        con->commit(); // ✅ success

        cout << " Deposit successful\n";

        delete log;
        delete pstmt;
        delete con;

    }
    catch (sql::SQLException &e){
    if(con){
        try {
            con->rollback();
        } catch(...) {}
    }
    cout << "Error: " << e.what() << endl;
}
}
void withdraw(int user_id) {
    
    double amount;
    string note;
    cout << "Enter note/description: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin,note);
    cout << "\n[Withdraw Money]\n";
    cout << "Enter Amount: ";
    if(!(cin >> amount)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid amount\n";
    return; // ✅ exit function
}

    if (amount <= 0) {
        cout << "Invalid amount\n";
        return;
    }
    sql::Connection *con = nullptr;
    try {
        con = getConnection();

        // Check user + balance
        sql::PreparedStatement *checkStmt = con->prepareStatement(
    "SELECT a.balance, u.is_frozen "
    "FROM accounts a "
    "JOIN users u ON a.user_id = u.id "
    "WHERE a.user_id = ?"
);
checkStmt->setInt(1, user_id);
        sql::ResultSet *res = checkStmt->executeQuery();

if (!res->next()) {
    cout << "User not found\n";
    delete res;
    delete checkStmt;
    delete con;
    return;
}

bool isFrozen = res->getBoolean(2);

if (isFrozen) {
    cout << "Account is frozen! cannot withdraw.\n";
    delete res;
    delete checkStmt;
    delete con;
    return;
}

        double balance = res->getDouble("balance");

        if (balance < amount) {
            cout << "Insufficient balance\n";
            delete res;
            delete checkStmt;
            delete con;
            return;
        }

        delete res;
        delete checkStmt;

        // Start transaction
        con->setAutoCommit(false);

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "UPDATE accounts SET balance = balance - ? WHERE user_id = ?"
        );

        pstmt->setDouble(1, amount);
        pstmt->setInt(2, user_id);

        pstmt->executeUpdate();

// 🔥 ADD TRANSACTION LOG
bool isFraud = (amount > 50000);

sql::PreparedStatement *log = con->prepareStatement(
    "INSERT INTO transactions (sender_id, receiver_id, type, amount, is_suspicious, description) VALUES (?, ?, 'WITHDRAW', ?, ?, ?)"
);

log->setInt(1, user_id);
log->setNull(2, sql::DataType::INTEGER);
log->setDouble(3, amount);
log->setBoolean(4, isFraud);
log->setString(5, note);

log->executeUpdate();

// ✅ COMMIT AFTER BOTH OPERATIONS
con->commit();

cout << "Withdrawal Successful!\n";

delete log;
        logAction("Withdraw: user_id=" + to_string(user_id) + 
          " amount=" + to_string(amount));
        // 🔥 Show updated balance
        sql::PreparedStatement *balStmt = con->prepareStatement(
            "SELECT balance FROM accounts WHERE user_id = ?"
        );

        balStmt->setInt(1, user_id);
        sql::ResultSet *balRes = balStmt->executeQuery();

        if (balRes->next()) {
            cout << "Remaining Balance: " << balRes->getDouble("balance") << endl;
        }

        delete balRes;
        delete balStmt;
        delete pstmt;
        delete con;

    } catch (sql::SQLException &e) {
    if(con){
        try {
            con->rollback();
        } 
        catch (...) {  
            // ignore rollback failure  
        }
    }
    cout << "Error: " << e.what() << endl;
    logAction("ERROR in withdraw: " + string(e.what()));
}
}
void transfer(int sender_id){

    string note;
    cout << "Enter note/description: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, note);

    // 🔥 SHOW USERS LIST
    vector<int> userIds;
    // 🔥 SHOW USERS LIST
try {
    sql::Connection *tempCon = getConnection();
    sql::Statement *stmt = tempCon->createStatement();
    sql::ResultSet *res = stmt->executeQuery(
        "SELECT id, name FROM users"
    );

    cout << "\n===== SELECT RECEIVER =====\n";
    int index = 1;

    while(res->next()){
        int id = res->getInt("id");
        string name = res->getString("name");

        cout << index << ". " << name << " (ID: " << id << ")\n";
        userIds.push_back(id);
        index++;
    }

    delete res;
    delete stmt;
    delete tempCon;

} catch(...) {
    cout << "Error loading users\n";
    return;
}

// SELECT RECEIVER
int choice;
cout << "Enter choice: ";
if(!(cin >> choice)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    return;
}

if(choice < 1 || choice > (int)userIds.size()){
    cout << "Invalid selection\n";
    return;
}

int receiver_id = userIds[choice - 1];

// AMOUNT
double amount;
cout << "Enter Amount: ";
if(!(cin >> amount)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    return;
}

if(amount <= 0){
    cout << "Invalid amount\n";
    return;
}

// PAYMENT METHOD
cout << "1. UPI\n2. WALLET\n3. BANK\n";
int methodChoice;
if(!(cin >> methodChoice)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    return;
}

string method;
switch(methodChoice){
    case 1: method = "UPI"; break;
    case 2: method = "WALLET"; break;
    case 3: method = "BANK"; break;
    default:
        cout << "Invalid method\n";
        return;
}

string detail = "N/A";
if(method == "UPI"){
    cout << "Enter UPI ID: ";
    cin >> detail;
}
else if(method == "BANK"){
    cout << "Enter bank name: ";
    cin >> detail;
}

// OTP
static bool seeded = false;
if(!seeded){
    srand(time(0));
    seeded = true;
}

int otp = rand() % 9000 + 1000;
cout << "OTP: " << otp << endl;
int enteredOtp;
cout << "Enter OTP: ";
if(!(cin >> enteredOtp)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid input\n";
    return;
}

if(enteredOtp != otp){
    cout << "Invalid OTP\n";
    return;
}

    sql::Connection *con = nullptr;

try {
    con = getConnection();

    // 🔥 CHECK SENDER
    sql::PreparedStatement *stmt1 = con->prepareStatement(
        "SELECT balance, (SELECT is_frozen FROM users WHERE id = ?) AS is_frozen FROM accounts WHERE user_id = ?"
    );

    stmt1->setInt(1, sender_id);
    stmt1->setInt(2, sender_id);

    sql::ResultSet *res1 = stmt1->executeQuery();

    if(!res1->next()){
        cout << "Sender not found\n";
        delete res1;
        delete stmt1;
        delete con;
        return;
    }

    if(res1->getBoolean("is_frozen")){
        cout << "Account is frozen\n";
        delete res1;
        delete stmt1;
        delete con;
        return;
    }

    double balance = res1->getDouble("balance");

    if(balance < amount){
        cout << " Insufficient balance\n";
        delete res1;
        delete stmt1;
        delete con;
        return;
    }

    delete res1;
    delete stmt1;

    // 🔥 CHECK RECEIVER
    sql::PreparedStatement *stmt2 = con->prepareStatement(
        "SELECT a.user_id FROM accounts a JOIN users u ON a.user_id = u.id WHERE a.user_id = ?"
    );

    stmt2->setInt(1, receiver_id);
    sql::ResultSet *res2 = stmt2->executeQuery();

    if(!res2->next()){
        cout << "Receiver not found\n";
        delete res2;
        delete stmt2;
        delete con;
        return;
    }

    delete res2;
    delete stmt2;

    // 🔥 START TRANSACTION
    con->setAutoCommit(false);
    bool isFraud = false;

    try{
        sql::PreparedStatement *p1 = con->prepareStatement(
            "UPDATE accounts SET balance = balance - ? WHERE user_id = ?"
        );
        p1->setDouble(1, amount);
        p1->setInt(2, sender_id);
        p1->executeUpdate();

        sql::PreparedStatement *p2 = con->prepareStatement(
            "UPDATE accounts SET balance = balance + ? WHERE user_id = ?"
        );
        p2->setDouble(1, amount);
        p2->setInt(2, receiver_id);
        p2->executeUpdate();

        if(amount > 50000){
            cout << " Large transaction detected!\n";
            isFraud = true;
        }

        sql::PreparedStatement *log = con->prepareStatement(
            "INSERT INTO transactions(sender_id,receiver_id,type,amount,payment_method,payment_details,is_suspicious,description) VALUES(?,?,'TRANSFER',?,?,?,?,?)"
        );

        log->setInt(1, sender_id);
        log->setInt(2, receiver_id);
        log->setDouble(3, amount);
        log->setString(4, method);
        log->setString(5, detail);
        log->setBoolean(6, isFraud);
        log->setString(7, note);

        log->executeUpdate();

        con->commit();

        cout << " Transfer successful!\n";

        delete p1;
        delete p2;
        delete log;

    } catch(...){
        try { con->rollback(); } catch(...) {}
        cout << " Transaction failed. Rolled back.\n";
    }

    delete con;

} catch(sql::SQLException &e){
    if(con){
        try { con->rollback(); } catch(...) {}
    }
    cout << "Error: " << e.what() << endl;
}
}
void viewTransactions(int user_id){

    int choice;
    string fromDate , toDate;

    cout << "\nFilter by Date? (y/n): ";
    char dateChoice;
    if(!(cin >> dateChoice)){
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Invalid method\n";
    return;
}

    if(dateChoice == 'y' || dateChoice == 'Y'){
        cout << "Enter from Date (YYYY-MM-DD): ";
        cin >> fromDate;
        cout << "Enter To Date (YYYY-MM-DD): ";
        cin >> toDate;
    }

    double minAmount = -1;
    double maxAmount = -1;

    cout << "\nFilter by Amount Range? (y/n): ";
    char amtChoice;
    cin >> amtChoice;

    if(amtChoice =='y' || amtChoice == 'Y'){
        cout << "Enter Min Amount: ";
        cin >> minAmount;
        cout << "Enter Max Amount: ";
        cin >> maxAmount;
    }

    int searchOption;
    cout << "\nSearch By:\n1. Default\n2. Search by Name\nEnter choice: ";
    cin >> searchOption;

    string searchName;
    if(searchOption == 2){
        cout << "Enter name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, searchName);
    }

    int page;
    cout << "Enter page number: ";
    cin >> page;

    int limit = 5;
    int offset = (page-1)*limit;

    cout << "1. All\n2. Deposit\n3. Withdraw\n4. Transfer\n";
    cout << "Enter choice: ";
    cin >> choice;

    try {
        sql::Connection *con = getConnection();

        string query =
        "SELECT t.*, s.name AS sender_name, r.name AS receiver_name "
        "FROM transactions t "
        "LEFT JOIN users s ON t.sender_id = s.id "
        "LEFT JOIN users r ON t.receiver_id = r.id "
        "WHERE 1=1";

        if (searchOption == 1) {
            query += " AND (t.sender_id = ? OR t.receiver_id = ?)";
        }
        else if (searchOption == 2) {
            query += " AND (s.name LIKE ? OR r.name LIKE ?)";
        }

        if (choice == 2) query += " AND type = 'DEPOSIT'";
        else if (choice == 3) query += " AND type = 'WITHDRAW'";
        else if (choice == 4) query += " AND type = 'TRANSFER'";
        else if (choice != 1) return;

        if (dateChoice == 'y' || dateChoice == 'Y') {
            query += " AND DATE(t.created_at) BETWEEN ? AND ?";
        }

        if (amtChoice == 'y' || amtChoice == 'Y') {
            query += " AND amount BETWEEN ? AND ?";
        }

        query += " ORDER BY created_at DESC LIMIT ? OFFSET ?";

        sql::PreparedStatement *pstmt = con->prepareStatement(query);

        int index = 1;

        if (searchOption == 1) {
            pstmt->setInt(index++, user_id);
            pstmt->setInt(index++, user_id);
        }
        else {
            pstmt->setString(index++, "%" + searchName + "%");
            pstmt->setString(index++, "%" + searchName + "%");
        }

        if (dateChoice == 'y' || dateChoice == 'Y') {
            pstmt->setString(index++, fromDate);
            pstmt->setString(index++, toDate);
        }

        if (amtChoice == 'y' || amtChoice == 'Y') {
            pstmt->setDouble(index++, minAmount);
            pstmt->setDouble(index++, maxAmount);
        }

        pstmt->setInt(index++, limit);
        pstmt->setInt(index++, offset);

        sql::ResultSet *res = pstmt->executeQuery();

        cout << "\n====== TRANSACTIONS ======\n";

        while(res->next()){

            string sender = res->isNull("sender_name") ? "SYSTEM" : res->getString("sender_name");
            string receiver = res->isNull("receiver_name") ? "SYSTEM" : res->getString("receiver_name");

            cout << sender << " → " << receiver
                 << " | " << res->getString("type")
                 << " | Rs " << res->getDouble("amount")
                 << " | " << res->getString("payment_method")
                 << endl;
        }

        delete res;
        delete pstmt;
        delete con;

    } catch (sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void CheckBalance(int user_id) {
    

    try{
       sql::Connection *con = getConnection();
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "SELECT balance FROM accounts WHERE user_id = ?"
        );

        pstmt->setInt(1,user_id);

        sql::ResultSet *res = pstmt->executeQuery();
        if(!res->next()){
            cout << "User not found\n";
        }
        else{
            double balance = res->getDouble("balance");
            cout << "Balance: " << balance << endl;
        }
        delete res;
        delete pstmt;
        delete con;
    }
    catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void showMonthlySpending(int user_id) {
    try {
        sql::Connection *con = getConnection();

        string query =
        "SELECT DATE_FORMAT(created_at, '%Y-%m') AS month, "
        "SUM(amount) AS total_spent "
        "FROM transactions "
        "WHERE sender_id = ? AND type IN ('WITHDRAW', 'TRANSFER') "
        "GROUP BY month "
        "ORDER BY month ASC";

        sql::PreparedStatement *pstmt = con->prepareStatement(query);
        pstmt->setInt(1, user_id);

        sql::ResultSet *res = pstmt->executeQuery();

        cout << "\n====== MONTHLY SPENDING ======\n";

        while (res->next()) {
            string month = res->getString("month");
            double total = res->getDouble("total_spent");

            cout << month << " → Rs " << total << endl;
        }

        delete res;
        delete pstmt;
        delete con;

    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}
void showAccountSummary(int user_id){
    try {
        sql::Connection *con = getConnection();

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "SELECT "
            "SUM(CASE WHEN type='DEPOSIT' THEN amount ELSE 0 END) AS total_deposit, "
            "SUM(CASE WHEN type='WITHDRAW' THEN amount ELSE 0 END) AS total_withdraw, "
            "SUM(CASE WHEN type='TRANSFER' AND sender_id=? THEN amount ELSE 0 END) AS total_sent "
            "FROM transactions WHERE sender_id=? OR receiver_id=?"
        );

        pstmt->setInt(1, user_id);
        pstmt->setInt(2, user_id);
        pstmt->setInt(3, user_id);

        sql::ResultSet *res = pstmt->executeQuery();

        if(res->next()){
            cout << "\n===== ACCOUNT SUMMARY =====\n";
            cout << "Total Deposits: Rs" << res->getDouble("total_deposit") << endl;
            cout << "Total Withdrawals: Rs" << res->getDouble("total_withdraw") << endl;
            cout << "Total Sent: Rs" << res->getDouble("total_sent") << endl;
        }

        delete res;
        delete pstmt;
        delete con;

    } catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}