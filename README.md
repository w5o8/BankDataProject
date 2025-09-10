# 🏦 Bank Management System (C++ Project)

A **console-based Bank Management System** built in **C++** that simulates core banking operations with persistent data storage in a text file.

---

## ✨ Features

### 👤 Client Management
- Add new clients (Account Number – PIN – Name – Phone – Balance).
- View all clients in a formatted table.
- Update client information.
- Delete clients safely (with confirmation).
- Search for a client by account number.

### 💰 Transactions
- Deposit money into a client account.
- Withdraw money with basic validation.
- Show all balances with a total balance summary.

### 📂 File Handling
- Stores all client records in `Bank.txt`.
- Supports loading, updating, and saving client data.
- Converts records between text lines and C++ structs.

---

## 🛠️ Tech Stack
- **Language**: C++
- **Concepts**: Structs, Enums, Vectors, File I/O, String Manipulation, Console Formatting
- **Data File**: `Bank.txt`

---

## 🚀 How to Run
1. Make sure `Bank.txt` exists in the project directory (create an empty file if it doesn't).
2. Compile the project with a C++ compiler, e.g.:
   ```bash
   g++ main.cpp -o BankSystem

📸 Demo (Console Output)
=============================
	Main Menu Screen
=============================
[ 1 ] Show Client List
[ 2 ] Add New Client
[ 3 ] Delete Client
[ 4 ] Update Client Info
[ 5 ] Find Client
[ 6 ] Transactions
[ 7 ] Exit
=============================
Choose [1 to 7]:
