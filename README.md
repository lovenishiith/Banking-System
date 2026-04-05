# 🏦 Banking System (C++ + MySQL)

A console-based banking system built using C++ and MySQL, simulating real-world banking operations with transaction safety, fraud detection, and admin controls.

---

## 🚀 Features

### 👤 User Management

* Create new user accounts
* Secure login with hashed passwords
* Account blocking after multiple failed login attempts
* Role-based access (USER / ADMIN)

---

### 💰 Banking Operations

* Deposit money
* Withdraw money
* Transfer money with OTP verification
* Real-time balance updates

---

### 📊 Transaction System

* View transaction history
* Filter by date and amount
* Pagination support
* Detailed transaction logs

---

### 🚨 Fraud Detection

* Automatically flags large transactions
* View suspicious transactions (Admin only)

---

### 🛠️ Admin Panel

* Freeze / Unfreeze user accounts
* View all users
* View blocked users
* Monitor suspicious transactions

---

### 📈 Analytics

* Account summary (total deposits, withdrawals, transfers)
* Monthly spending analysis

---

## 🧠 Concepts Used

* Database Transactions (ACID properties)
* Prepared Statements (SQL Injection safe)
* Password Hashing
* Role-Based Access Control
* Error Handling & Input Validation
* Modular Code Structure

---

## 🏗️ Project Structure

```
BankingSystem/
│
├── main.cpp
├── db.cpp
├── user.cpp
├── transaction.cpp
├── dashboard.cpp
├── utils.cpp
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

```
mysql -u root -p < banking.sql
```

---

### 3. Compile

```
cl main.cpp db.cpp user.cpp transaction.cpp showDashboard.cpp utils.cpp ^
/I "C:\mysql\connector\include" ^
/link /LIBPATH:"C:\mysql\connector\lib64\vs14" mysqlcppconn.lib
```

---

### 4. Run

```
main.exe
```

---

## 🔐 Default Behavior

* First registered user becomes ADMIN
* Other users are USER

---

## 🧪 Testing

* Invalid input handling
* OTP verification
* Fraud detection (> 50000 transactions)
* Insufficient balance checks
* Filtering and pagination

---

## 📌 Future Improvements

* Convert to REST API
* Add GUI / Web interface
* Improve security (salted hashing)
* Multi-threading support
* Authentication tokens

---

## 👨‍💻 Author

Lovenish 
B.Tech, IIT Hyderabad

---

