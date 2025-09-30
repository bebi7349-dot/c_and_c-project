#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Account {
    int accountNumber;
    string name;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Initial Balance: ";
        cin >> balance;
        cout << "\n✅ Account Created Successfully!\n";
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << "\n";
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << "\n❌ Insufficient Balance!\n";
        } else {
            balance -= amt;
        }
    }

    int getAccNo() const { return accountNumber; }
    double getBalance() const { return balance; }
    string getName() const { return name; }

    void report() const {
        cout << setw(10) << accountNumber << setw(20) << name << setw(15) << balance << endl;
    }
};

// ---------- File Handling ----------
void saveAccount(Account acc) {
    ofstream outFile("data.txt", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

void displayAll() {
    Account acc;
    ifstream inFile("data.txt", ios::binary);
    if (!inFile) {
        cout << "\n❌ File could not be opened!\n";
        return;
    }
    cout << "\n\nACCOUNT LIST\n";
    cout << "====================================================\n";
    cout << setw(10) << "Acc No" << setw(20) << "Name" << setw(15) << "Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.report();
    }
    inFile.close();
}

void displayAccount(int n) {
    Account acc;
    ifstream inFile("data.txt", ios::binary);
    if (!inFile) {
        cout << "\n❌ File could not be opened!\n";
        return;
    }
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == n) {
            acc.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found) cout << "\n❌ Account not found!\n";
}

void depositWithdraw(int n, int option) {
    Account acc;
    fstream file("data.txt", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\n❌ File could not be opened!\n";
        return;
    }

    bool found = false;
    while (!file.eof() && found == false) {
        streampos pos = file.tellg();
        if (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
            if (acc.getAccNo() == n) {
                acc.showAccount();
                double amt;
                if (option == 1) {
                    cout << "\nEnter amount to deposit: ";
                    cin >> amt;
                    acc.deposit(amt);
                }
                if (option == 2) {
                    cout << "\nEnter amount to withdraw: ";
                    cin >> amt;
                    acc.withdraw(amt);
                }
                file.seekp(pos);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
                cout << "\n✅ Transaction Successful!\n";
                found = true;
            }
        }
    }
    file.close();
    if (!found) cout << "\n❌ Account not found!\n";
}

// ---------- Main Menu ----------
int main() {
    int choice, num;
    Account acc;

    do {
        cout << "\n\n===== BANK MANAGEMENT SYSTEM =====";
        cout << "\n1. Create New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Balance Inquiry";
        cout << "\n5. All Account Holders List";
        cout << "\n6. Exit";
        cout << "\n==================================";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            acc.createAccount();
            saveAccount(acc);
            break;
        case 2:
            cout << "\nEnter Account No: "; cin >> num;
            depositWithdraw(num, 1);
            break;
        case 3:
            cout << "\nEnter Account No: "; cin >> num;
            depositWithdraw(num, 2);
            break;
        case 4:
            cout << "\nEnter Account No: "; cin >> num;
            displayAccount(num);
            break;
        case 5:
            displayAll();
            break;
        case 6:
            cout << "\n🙏 Thank you for using Bank Management System!\n";
            break;
        default:
            cout << "\n❌ Invalid Choice!\n";
        }
    } while (choice != 6);

    return 0;
}
