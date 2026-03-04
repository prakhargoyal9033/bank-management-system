# 🏦 Bank Management System (C Language)

A simple **Bank Management System** built using the C programming language.  
This project allows users to create accounts, perform transactions, and store data securely using file handling and basic encryption.

---

## 📌 Features

- Create Single Account  
- Create Joint Account (Up to 2 Owners)  
- Deposit Money  
- Withdraw Money  
- View Account Details  
- View All Accounts  
- Transaction History with Timestamp  
- Close Account  
- Erase All Accounts  
- Persistent Data Storage (Binary File)  
- PIN Encryption using XOR Cipher  

---

## 🛠 Technologies Used

- C Programming Language
- Standard Libraries:
  - stdio.h
  - stdlib.h
  - string.h
  - time.h

---

## 📂 Project Structure

```
.
├── main.c
├── bankdat.dat (auto-generated)
└── README.md
```

---

## 🔐 Security Implementation

Each account has a 4-digit PIN.

PINs are encrypted using a simple XOR cipher:

```c
#define KEY 7

void encryptDecrypt(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] ^= KEY;
}
```

⚠ Note: This encryption is for learning purposes only and not secure for real-world banking systems.

---

## 💾 Data Storage

- Data is stored in a binary file named:

```
bankdat.dat
```

- Automatically loads data when the program starts.
- Automatically saves data when the program exits.

---

## 🧾 Account Details Stored

Each account contains:

- Account Number
- Owner Name(s)
- PIN(s)
- Balance
- Transaction History
- Timestamp for each transaction

---

## ▶ How to Compile and Run

### Using GCC (Linux / Mac)

```bash
gcc main.c -o bank
./bank
```

### Using GCC (Windows - MinGW)

```bash
gcc main.c -o bank.exe
bank.exe
```

---

## 📋 Main Menu

```
===== Bank Management System =====
1. Create Single Account
2. Create Joint Account
3. Deposit Money
4. Withdraw Money
5. Display Account Details
6. Display All Accounts
7. Close Account
8. Erase All Accounts
9. Exit
```

---

## 📊 Sample Transaction History

```
Type                 Amount       Time
-----------------------------------------------------------
Initial Deposit      1000.00      Wed Mar 05 21:34:10 2026
Deposit              500.00       Wed Mar 05 21:40:12 2026
Withdraw             200.00       Wed Mar 05 21:45:05 2026
```

---

## ⚙ Limitations

- Maximum 100 accounts
- Maximum 2 owners per account
- Maximum 100 transactions per account
- Basic encryption only
- Limited input validation

---

## 🚀 Future Improvements

- Stronger encryption (hashing)
- Input validation improvements
- Admin authentication
- Search account by name
- Interest calculation
- GUI version
- Database integration (MySQL / SQLite)

---

## 📚 Concepts Used

- Structures in C
- File Handling (Binary Files)
- Arrays of Structures
- Pointers
- Encryption (XOR Cipher)
- Time and Date Handling
- Menu-driven Programming

---

## 👨‍💻 Author

prakhar goyal
