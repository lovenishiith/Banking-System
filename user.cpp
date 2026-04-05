#include "user.h"
#include "db.h"
#include "utils.h"
#include <iostream>
#include <limits>
using namespace std;


UserSession loginUser(){
    string email , password;
cin.ignore(numeric_limits<streamsize>::max(), '\n');    cout << "\n===== LOGIN =====\n";
    cout << "Enter Email: ";
    getline(cin,email);
    cout << "Enter Password: ";
    getline(cin,password);
    logAction("Login attempt for email: " + email);

    try {
       sql::Connection *con = getConnection();

        sql::PreparedStatement *pstmt = con->prepareStatement(
           "SELECT id, name, email,role,failed_attempts, is_blocked ,password FROM users WHERE email = ?"
        );

        pstmt->setString(1, email);

        sql::ResultSet *res = pstmt->executeQuery();
        if(res->next()){
            bool isBlocked = res->getBoolean("is_blocked");
            int attempts = res->getInt("failed_attempts");
            string storedPassword = res->getString("password");

            if(isBlocked){
    cout << "Account is blocked! Contact admin.\n";
    delete res;
    delete pstmt;
    delete con;
    return {-1,"","",""};
}

            string hashed = hashPassword(password);
            if(storedPassword == hashed){
                // reset attempts
                sql::PreparedStatement *resetStmt = con->prepareStatement(
                    "UPDATE users SET failed_attempts = 0 WHERE email = ?"
                );
                resetStmt->setString(1,email);
                resetStmt->executeUpdate();
                delete resetStmt;

                UserSession user;
                user.id = res->getInt("id");
                user.name = res->getString("name");
                user.email = res->getString("email");
                user.role = res->getString("role");

                cout << "Login successful! User: " << user.name << endl;           
                logAction("Login success for user_id=" + to_string(user.id));

                delete res;
                delete pstmt;
                delete con;

                return user;
            }
            else {
                attempts++;

                if(attempts >= 3){
                    sql::PreparedStatement *blockStmt = con->prepareStatement(
                        "UPDATE users SET failed_attempts = ? , is_blocked = TRUE WHERE email = ?"
                    );
                    blockStmt->setInt(1,attempts);
                    blockStmt->setString(2,email);
                    blockStmt->executeUpdate();
                    delete blockStmt;

                    cout << "Account blocked due to multiple failed attempts!\n";
                }
                else{
                    sql::PreparedStatement *updateStmt = con->prepareStatement(
                        "UPDATE users SET failed_attempts = ? WHERE email = ?"
                    );
                    updateStmt->setInt(1,attempts);
                    updateStmt->setString(2,email);
                    updateStmt->executeUpdate();
                    delete updateStmt;
                    
                    cout << "Invalid password! Attempts left: " << (3 - attempts) << endl;
                }
                logAction("Login failed for email=" + email);

                delete res;
                delete pstmt;
                delete con;

                return {-1,"","",""};
            }
        }
        else {
cout << "Invalid credentials\n";
            logAction("Login failed for email: " + email);

            delete res;
            delete pstmt;
            delete con;

            return {-1, "", "",""};
        }

    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
        logAction("ERROR in loginUser: " + string(e.what()));
        return {-1,"","",""};
    }
}
void createUser(){
    string name , email ,gender,dob,password; 

cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Name: ";
    getline(cin,name);
    cout << "Enter Email: ";
    getline(cin,email);
    cout << "Enter Gender: ";
    getline(cin,gender);
    cout << "Enter DOB (YYYY-MM-DD): ";
    getline(cin,dob);
    cout << "Enter Password: ";
    getline(cin,password);

    try {
      sql::Connection *con = getConnection();
        // 🔥 CHECK IF EMAIL EXISTS
sql::PreparedStatement *checkStmt = con->prepareStatement(
    "SELECT id FROM users WHERE email = ?"
);

checkStmt->setString(1, email);

// ✅ CORRECT RESULTSET
sql::ResultSet *checkRes = checkStmt->executeQuery();

if(checkRes->next()){
    cout << " Email already exists! Please login instead.\n";
    
    delete checkRes;
    delete checkStmt;
    delete con;
    return;
}

// ✅ CLEANUP
delete checkRes;
delete checkStmt;
    string role = "USER";

// 🔥 Make FIRST USER admin
sql::PreparedStatement *checkAdmin = con->prepareStatement(
    "SELECT COUNT(*) FROM users WHERE role='ADMIN'"
);

sql::ResultSet *resAdmin = checkAdmin->executeQuery();

if(resAdmin->next() && resAdmin->getInt(1) == 0){
    role = "ADMIN";
}

delete resAdmin;
delete checkAdmin;
        // INSERT USER
        sql::PreparedStatement *pstmt = con->prepareStatement(
"INSERT INTO users (name,email,gender,date_of_birth,password,role) VALUES (?,?,?,?,?,?)"        );

        pstmt->setString(1,name);
        pstmt->setString(2,email);
        pstmt->setString(3,gender);
        pstmt->setString(4,dob);
        string hashed = hashPassword(password);
        pstmt->setString(5,hashed);
        pstmt->setString(6,role);
        pstmt->executeUpdate();
        delete pstmt;

        // get ID
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

        int user_id = 0;
        if(res->next()){
            user_id = res->getInt(1);
        }

        delete res;
        delete stmt;

        // create account 
        pstmt = con->prepareStatement(
            "INSERT INTO accounts (user_id,balance) VALUES (?,0)"
        );
        pstmt->setInt(1,user_id);
        pstmt->executeUpdate();

        delete pstmt;
        delete con;

        cout << "User created successfully! ID: " << user_id << endl;
        logAction("User created: ID=" + to_string(user_id) + " email=" + email);
    }
    catch (sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
        logAction("ERROR in createUser: " + string(e.what()));
    }
}
void freezeUser(){
    int user_id;
    cout << "Enter user ID to freeze: ";
    cin >> user_id;
    try{
        sql::Connection *con = getConnection();
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "UPDATE users SET is_frozen = TRUE WHERE id = ? "
        );
        pstmt->setInt(1,user_id);
        int rows = pstmt->executeUpdate();
        if(rows > 0){
            cout << "User frozen successfully!\n";
        }
        else{
            cout << "User not found\n";
        }
        delete pstmt;
        delete con;

    }
    catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void unfreezeUser(){
    int user_id;
    cout << "Enter user ID to unfreeze: ";
    cin >> user_id;

    try{
        sql::Connection *con = getConnection();

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "UPDATE users SET is_frozen = FALSE WHERE id = ?"
        );

        pstmt->setInt(1, user_id);

        int rows = pstmt->executeUpdate();

        if(rows > 0){
            cout << "User unfrozen successfully!\n";
        } else {
            cout << "User not found\n";
        }

        delete pstmt;
        delete con;

    } catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void viewAllUsers(){
    try{
        sql::Connection *con = getConnection();
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(
            "SELECT id,name,email,role FROM users"
        );
        
        cout << "\n===== ALL USERS =====\n";
        while(res->next()){
            cout << "ID: " << res->getInt("id")
                 << " Name: " << res->getString("name")
                 << " Email: " << res->getString("email")
                 << " Role: " << res->getString("role") << endl;
        }
        delete res;
        delete stmt;
        delete con;
    }
    catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}

void viewBlockedUsers(){
    try{
        sql::Connection *con = getConnection();

        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(
            "SELECT id, name, email FROM users WHERE is_blocked = TRUE"
        );

        cout << "\n===== BLOCKED USERS =====\n";

        while(res->next()){
            cout << "ID: " << res->getInt("id")
                 << " Name: " << res->getString("name")
                 << " Email: " << res->getString("email") << endl;
        }

        delete res;
        delete stmt;
        delete con;

    } catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void unblockUser(){
    int user_id;
    cout << "Enter user ID to unblock: ";
    cin >> user_id;

    try{
        sql::Connection *con = getConnection();

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "UPDATE users SET is_blocked =FALSE , failed_attempts = 0 WHERE id = ? "
        );
        pstmt->setInt(1,user_id);

        int rows = pstmt->executeUpdate();
        if(rows > 0){
            cout << "User unblocked successfully!\n";
        }
        else{
            cout << "User not found\n";
        }
        delete pstmt;
        delete con;
    }
    catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}
void viewSuspiciousTransactions(){
    try{
        sql::Connection *con = getConnection();

        sql::PreparedStatement *pstmt = con->prepareStatement(
            "SELECT t.*, u1.name AS sender_name, u2.name AS receiver_name "
            "FROM transactions t "
            "LEFT JOIN users u1 ON t.sender_id = u1.id "
            "LEFT JOIN users u2 ON t.receiver_id = u2.id "
            "WHERE t.is_suspicious = TRUE"
        );

        sql::ResultSet *res = pstmt->executeQuery();

        cout << "\n====== FRAUD ALERTS =======\n";

        while(res->next()){
            string sender = res->isNull("sender_name") ? "SYSTEM" : res->getString("sender_name");
            string receiver = res->isNull("receiver_name") ? "SYSTEM" : res->getString("receiver_name");

            cout << "From: " << sender
                 << " → To: " << receiver
                 << " Amount: " << res->getDouble("amount")
                 << " Method: " << res->getString("payment_method")
                 << endl;
        }

        delete res;
        delete pstmt;
        delete con;

    } catch(sql::SQLException &e){
        cout << "Error: " << e.what() << endl;
    }
}