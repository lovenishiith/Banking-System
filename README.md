# 🏦 Banking Management System (C++ + MySQL)

A console-based Banking Management System built using C++ and MySQL, designed to simulate real-world banking operations with secure money transfer, transaction safety, fraud detection, admin controls, filtering, analytics, and a structured banking dashboard.

This project focuses on making banking operations realistic using ACID-compliant database transactions, prepared statements for security, and role-based access control for user and admin functionalities.

---

## 🚀 Features

---

### 👤 User Management

- Create new user accounts
- Secure login system
- Account blocking after multiple failed login attempts
- Role-based access control (USER / ADMIN)
- First registered user becomes ADMIN automatically

---

### 💰 Banking Operations

- Deposit money
- Withdraw money
- Transfer money with OTP verification
- Real-time balance updates
- Account freeze protection during transactions
- Proper balance validation before withdrawal and transfer

---

### 🔐 Transaction Safety

- ACID-compliant money transfer using MySQL transactions
- Rollback support if transfer fails
- Prevents partial debit/credit updates
- Safe transaction handling using `setAutoCommit(false)` + `commit()` + `rollback()`

---

### 📊 Advanced Transaction System

- View transaction history
- Filter by date range
- Filter by amount range
- Search transactions by user name
- Pagination support
- Detailed transaction logs
- Transaction ID display
- Suspicious transaction status display
- Clean formatted transaction history output

---

### 🚨 Fraud Detection

- Automatically flags large transactions (> 50000)
- Suspicious transaction monitoring
- Admin can review suspicious transactions
- Helps simulate real-world fraud detection systems

---

### 🛠️ Admin Panel

- View all users
- View blocked users
- Freeze user accounts
- Unfreeze user accounts
- Unblock users after failed login attempts
- Monitor suspicious transactions

---

### 📈 Analytics

- Account summary
  - Total deposits
  - Total withdrawals
  - Total transfers
- Monthly spending analysis
- User transaction insights

---

### 🎨 Improved Console UI

- Structured banking dashboard
- Clean grouped menu sections
- Welcome screen after login
- Better success and error messages
- Professional transaction display format
- Improved user experience for console interaction

---

## 🧠 Key Concepts Used

- MySQL Database Integration
- Database Transactions (ACID Properties)
- Prepared Statements (SQL Injection Safe)
- Fraud Detection Logic
- Role-Based Access Control
- Input Validation
- Pagination + Filtering
- Exception Handling
- Memory Management
- Modular Code Structure
- Admin Authorization Logic

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
