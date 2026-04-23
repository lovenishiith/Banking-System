# 🏦 Banking Management System (C++ + MySQL)

A console-based Banking Management System built using C++ and MySQL, designed to simulate real-world banking operations with secure money transfer, transaction safety, fraud detection, admin controls, filtering, analytics, and a structured banking dashboard.

This project focuses on making banking operations realistic using ACID-compliant database transactions, prepared statements for security, and role-based access control for user and admin functionalities.

---

## 🚀 Features

### 👤 User Management

* Create new user accounts
* Secure login system
* Account blocking after multiple failed login attempts
* Role-based access control (USER / ADMIN)
* First registered user becomes ADMIN automatically

---

### 💰 Banking Operations

* Deposit money
* Withdraw money
* Transfer money with OTP verification
* Real-time balance updates
* Account freeze protection during transactions
* Proper balance validation before withdrawal and transfer

---

### 🔐 Transaction Safety

* ACID-compliant money transfer using MySQL transactions
* Rollback support if transfer fails
* Prevents partial debit/credit updates
* Safe transaction handling using `setAutoCommit(false)` + `commit()` + `rollback()`

---

### 📊 Advanced Transaction System

* View transaction history
* Filter by date range
* Filter by amount range
* Search transactions by user name
* Pagination support
* Detailed transaction logs
* Transaction ID display
* Suspicious transaction status display
* Clean formatted transaction history output

---

### 🚨 Fraud Detection

* Automatically flags large transactions (> 50000)
* Suspicious transaction monitoring
* Admin can review suspicious transactions
* Helps simulate real-world fraud detection systems

---

### 🛠️ Admin Panel

* View all users
* View blocked users
* Freeze user accounts
* Unfreeze user accounts
* Unblock users after failed login attempts
* Monitor suspicious transactions

---

### 📈 Analytics

* Account summary

  * Total deposits
  * Total withdrawals
  * Total transfers
* Monthly spending analysis
* User transaction insights

---

### 🎨 Improved Console UI

* Structured banking dashboard
* Clean grouped menu sections
* Welcome screen after login
* Better success and error messages
* Professional transaction display format
* Improved user experience for console interaction

---

## 🧠 Key Concepts Used

* MySQL Database Integration
* Database Transactions (ACID Properties)
* Prepared Statements (SQL Injection Safe)
* Fraud Detection Logic
* Role-Based Access Control
* Input Validation
* Pagination + Filtering
* Exception Handling
* Memory Management
* Modular Code Structure
* Admin Authorization Logic

---

## 🏗️ Project Structure

```text
BankingSystem/
│
├── main.cpp
├── db.cpp
├── user.cpp
├── transaction.cpp
├── dashboard.cpp
├── utils.cpp
│
├── transaction.h
├── user.h
├── dashboard.h
├── utils.h
├── db.h
│
└── banking.sql
```

---

## ⚙️ Setup Instructions

### 1. Install Requirements

* MySQL Server
* MySQL Connector/C++
* C++ Compiler (Visual Studio recommended)

---

### 2. Setup Database

Run the SQL file to create tables and required schema:

```sql
mysql -u root -p < banking.sql
```

---

### 3. Compile Project

```bash
cl main.cpp db.cpp user.cpp transaction.cpp dashboard.cpp utils.cpp ^
/I "C:\mysql\connector\include" ^
/link /LIBPATH:"C:\mysql\connector\lib64\vs14" mysqlcppconn.lib
```

---

### 4. Run Project

```bash
main.exe
```

---

## 🔐 Default Behavior

* First registered user becomes ADMIN
* All other registered users become USER
* Large transactions are automatically marked suspicious
* Frozen users cannot perform withdrawals or transfers

---

## 🧪 Testing Covered

* Invalid input handling
* OTP verification system
* Fraud detection (> 50000 transactions)
* Insufficient balance checks
* Failed transaction rollback
* Account freeze restrictions
* Transaction filtering and pagination
* Admin access validation
* Login failure handling

---

## 📌 Future Improvements

* Convert project into REST API backend
* Add GUI / Web Interface
* Improve password security using salted hashing
* Add multi-threading for concurrent transactions
* Add authentication tokens
* Add email/SMS OTP verification
* Export transaction reports as PDF

---

## 💡 Resume Description

Built a console-based Banking Management System using C++ and MySQL with secure money transfer, fraud detection, transaction filtering, admin controls, and ACID-compliant transaction safety to simulate real-world banking operations.

---

## 👨‍💻 Author

**Lovenish**
B.Tech, IIT Hyderabad

---
