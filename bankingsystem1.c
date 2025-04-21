#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accountNo;
    char name[50];
    float balance;
};

void createAccount();
void viewAccount();
void depositMoney();
void withdrawMoney();
void deleteAccount();
void menu();

void createAccount() {
    FILE *fp = fopen("accounts.dat", "ab");
    struct Account acc;

    printf("\n--- Create Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc.accountNo);
    getchar();
    printf("Enter Name: ");
    gets(acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("Account Created Successfully.\n");
}

void viewAccount() {
    FILE *fp = fopen("accounts.dat", "rb");
    struct Account acc;

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");

    int found = 0;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("Acc No: %d | Name: %s | Balance: R.S : %.2f\n", acc.accountNo, acc.name, acc.balance);
        found = 1;
    }

    if (!found) {
        printf("No account data available.\n");
    }

    fclose(fp);
}

void depositMoney() {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNo;
    float amount;

    printf("\n--- Deposit Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accNo) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);

            printf("Deposit Successful. New Balance: R.S : %.2f\n", acc.balance);
            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

void withdrawMoney() {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct Account acc;
    int accNo;
    float amount;

    printf("\n--- Withdraw Money ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo == accNo) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);

                printf("Withdrawal Successful. New Balance: R.S : %.2f\n", acc.balance);
            } else {
                printf("Insufficient Balance.\n");
            }
            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

void deleteAccount() {
    FILE *fp = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Account acc;
    int accNo;
    int found = 0;

    printf("\n--- Delete Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNo != accNo) {
            fwrite(&acc, sizeof(acc), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        printf("Account Deleted Successfully.\n");
    else
        printf("Account not found.\n");
}

void menu() {
    int choice;

    do {
        printf("\n====== Banking Management System ======\n");
        printf("1. Create Account\n");
        printf("2. View All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: deleteAccount(); break;
            case 6: printf("Exiting. Thank you!\n"); exit(0);
            default: printf("Invalid choice. Try again.\n");
        }

    } while (1);
}

int main() {
    menu();
    return 0;
}

