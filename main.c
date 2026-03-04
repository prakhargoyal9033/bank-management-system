#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATA_FILE "bankdat.dat"
#define MAX_NAME 50
#define MAX_OWNERS 2
#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100
#define KEY 7

typedef struct {
    char type[20];
    float amount;
    char timeStr[30];
} Transaction;

typedef struct {
    int accNo;
    char name[MAX_OWNERS][MAX_NAME];
    char pin[MAX_OWNERS][10];
    int ownerCount;
    float balance;
    Transaction history[MAX_TRANSACTIONS];
    int transCount;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// -------- Utility --------
void encryptDecrypt(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] ^= KEY;
}

// -------- Find Account --------
int findAccountIndex(int accNo) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accNo == accNo)
            return i;
    }
    return -1;
}

// -------- Add Transaction --------
void addTransaction(Account *acc, const char *type, float amount) {
    if (acc->transCount >= MAX_TRANSACTIONS) return;
    Transaction *t = &acc->history[acc->transCount++];
    strcpy(t->type, type);
    t->amount = amount;
    time_t now = time(NULL);
    strcpy(t->timeStr, ctime(&now));
    t->timeStr[strcspn(t->timeStr, "\n")] = 0;
}

// -------- Show Transaction History --------
void showTransactions(Account *acc) {
    printf("Transaction History:\n");
    printf("%-20s %-12s %-30s\n", "Type", "Amount", "Time");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < acc->transCount; i++) {
        printf("%-20s %-12.2f %-30s\n",
               acc->history[i].type,
               acc->history[i].amount,
               acc->history[i].timeStr);
    }
}

// -------- Verify PIN --------
int verifyPIN(Account *acc) {
    char pin[10];
    printf("Enter PIN: ");
    scanf("%s", pin);
    for (int i = 0; i < acc->ownerCount; i++) {
        if (strcmp(acc->pin[i], pin) == 0)
            return 1;
    }
    printf("Incorrect PIN.\n");
    return 0;
}

// -------- Create Single Account --------
void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts.\n");
        return;
    }

    Account acc;
    acc.transCount = 0;
    acc.ownerCount = 1;

    printf("Enter Account Number: ");
    scanf("%d", &acc.accNo);
    getchar();

    if (findAccountIndex(acc.accNo) != -1) {
        printf("Account number already exists.\n");
        return;
    }

    printf("Enter Name: ");
    fgets(acc.name[0], MAX_NAME, stdin);
    acc.name[0][strcspn(acc.name[0], "\n")] = 0;

    printf("Set 4-digit PIN: ");
    scanf("%s", acc.pin[0]);

    printf("Enter Initial Deposit: ");
    scanf("%f", &acc.balance);

    addTransaction(&acc, "Initial Deposit", acc.balance);

    accounts[accountCount++] = acc;
    printf("Account created successfully!\n");
}

// -------- Create Joint Account --------
void createJointAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts.\n");
        return;
    }

    Account acc;
    acc.transCount = 0;

    printf("Enter Account Number: ");
    scanf("%d", &acc.accNo);
    getchar();

    if (findAccountIndex(acc.accNo) != -1) {
        printf("Account number already exists.\n");
        return;
    }

    printf("Enter number of owners (1 or 2): ");
    scanf("%d", &acc.ownerCount);
    getchar();

    if (acc.ownerCount < 1 || acc.ownerCount > 2) {
        printf("Invalid number of owners.\n");
        return;
    }

    for (int i = 0; i < acc.ownerCount; i++) {
        printf("Enter Name of Owner %d: ", i + 1);
        fgets(acc.name[i], MAX_NAME, stdin);
        acc.name[i][strcspn(acc.name[i], "\n")] = 0;

        printf("Set 4-digit PIN for %s: ", acc.name[i]);
        scanf("%s", acc.pin[i]);
        getchar();
    }

    printf("Enter Initial Deposit: ");
    scanf("%f", &acc.balance);

    addTransaction(&acc, "Initial Deposit", acc.balance);
    accounts[accountCount++] = acc;

    printf("Joint account created successfully!\n");
}

// -------- Deposit Money --------
void depositMoney() {
    int accNo;
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    if (!verifyPIN(&accounts[idx]))
        return;

    float amt;
    printf("Enter Amount to Deposit: ");
    scanf("%f", &amt);

    accounts[idx].balance += amt;
    addTransaction(&accounts[idx], "Deposit", amt);
    printf("%.2f deposited successfully. Current balance: %.2f\n", amt, accounts[idx].balance);
}

// -------- Withdraw Money --------
void withdrawMoney() {
    int accNo;
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    if (!verifyPIN(&accounts[idx]))
        return;

    float amt;
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amt);

    if (amt > accounts[idx].balance) {
        printf("Insufficient balance!\n");
        return;
    }

    accounts[idx].balance -= amt;
    addTransaction(&accounts[idx], "Withdraw", amt);
    printf("%.2f withdrawn successfully. Remaining balance: %.2f\n", amt, accounts[idx].balance);
}

// -------- Display Account --------
void displayAccount() {
    int accNo;
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    if (!verifyPIN(&accounts[idx]))
        return;

    Account *acc = &accounts[idx];
    printf("\n====== Account Details ======\n");
    printf("Account No: %d\n", acc->accNo);
    printf("Owners: ");
    for (int i = 0; i < acc->ownerCount; i++)
        printf("%s%s", acc->name[i], (i < acc->ownerCount - 1) ? ", " : "\n");
    printf("Balance: %.2f\n", acc->balance);
    printf("Number of Owners: %d\n", acc->ownerCount);
    printf("-------------------------------\n");
    showTransactions(acc);
}

// -------- Display All Accounts --------
void displayAllAccounts() {
    if (accountCount == 0) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n%-10s %-35s %-12s %-8s\n", "AccNo", "Owners", "Balance", "Owners#");
    printf("-------------------------------------------------------------------\n");
    for (int i = 0; i < accountCount; i++) {
        Account *acc = &accounts[i];
        printf("%-10d ", acc->accNo);
        for (int j = 0; j < acc->ownerCount; j++) {
            printf("%s", acc->name[j]);
            if (j < acc->ownerCount - 1)
                printf(", ");
        }
        printf("%*s%-12.2f %-8d\n", (int)(35 - strlen(acc->name[0]) - (acc->ownerCount > 1 ? strlen(acc->name[1]) + 2 : 0)), "",
               acc->balance, acc->ownerCount);
    }
}

// -------- Close Account --------
void closeAccount() {
    int accNo;
    printf("Enter Account Number to close: ");
    scanf("%d", &accNo);

    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPIN(&accounts[idx]))
        return;

    for (int i = idx; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--;

    printf("Account closed successfully.\n");
}

// -------- Erase All Accounts --------
void eraseAllAccounts() {
    char confirm;
    printf("Are you sure you want to erase ALL data? (Y/N): ");
    scanf(" %c", &confirm);
    if (confirm != 'Y' && confirm != 'y') {
        printf("Operation cancelled.\n");
        return;
    }
    accountCount = 0;
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp) fclose(fp);
    printf("All accounts erased successfully.\n");
}

// -------- Save / Load --------
void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf("Error saving data.\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        Account temp = accounts[i];
        for (int j = 0; j < temp.ownerCount; j++)
            encryptDecrypt(temp.pin[j]);
        fwrite(&temp, sizeof(Account), 1, fp);
    }

    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return;

    accountCount = 0;
    while (fread(&accounts[accountCount], sizeof(Account), 1, fp) == 1) {
        for (int j = 0; j < accounts[accountCount].ownerCount; j++)
            encryptDecrypt(accounts[accountCount].pin[j]);
        accountCount++;
    }
    fclose(fp);
}

// -------- Main Menu --------
int main() {
    loadFromFile();
    int choice;
    while (1) {
        printf("\n===== Bank Management System =====\n");
        printf("1. Create Single Account\n");
        printf("2. Create Joint Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Display Account Details\n");
        printf("6. Display All Accounts\n");
        printf("7. Close Account\n");
        printf("8. Erase All Accounts\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: createAccount(); break;
        case 2: createJointAccount(); break;
        case 3: depositMoney(); break;
        case 4: withdrawMoney(); break;
        case 5: displayAccount(); break;
        case 6: displayAllAccounts(); break;
        case 7: closeAccount(); break;
        case 8: eraseAllAccounts(); break;
        case 9:
            saveToFile();
            printf("Data saved. Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
        saveToFile();
    }
    return 0;
}
