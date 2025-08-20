#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Transaction {
private:
    string type;
    double amount;
    time_t timestamp;

public:
    Transaction(string t, double a) : type(t), amount(a), timestamp(time(0)) {}

    void displayTransaction() const {
        cout << "Type: " << type
             << ", Amount: PKR " << amount
             << ", Date: " << ctime(&timestamp);
    }
};

class Account {
private:
    static int accountCounter;
    int accountNumber;
    double balance;
    vector<Transaction> transactions;

public:
    Account() : balance(0.0) {
        accountNumber = ++accountCounter;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
        cout << "Deposited PKR " << amount << " to account #" << accountNumber << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Invalid withdrawal amount.\n";
            return;
        }
        balance -= amount;
        transactions.push_back(Transaction("Withdrawal", amount));
        cout << "Withdrew PKR " << amount << " from account #" << accountNumber << endl;
    }

    bool transferTo(Account& recipient, double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Transfer failed: insufficient funds.\n";
            return false;
        }
        balance -= amount;
        recipient.balance += amount;
        transactions.push_back(Transaction("Transfer Out", amount));
        recipient.transactions.push_back(Transaction("Transfer In", amount));
        cout << "Transferred PKR " << amount << " from account #" << accountNumber
             << " to account #" << recipient.accountNumber << endl;
        return true;
    }

    void displayAccountInfo() const {
        cout << "\nAccount Number: " << accountNumber
             << "\nBalance: PKR " << balance
             << "\nRecent Transactions:\n";
        for (const auto& t : transactions) {
            t.displayTransaction();
        }
        cout << endl;
    }
};
int Account::accountCounter = 1000;

class Customer {
private:
    static int customerCounter;
    int customerID;
    string name;
    vector<Account> accounts;

public:
    Customer(string n) : name(n) {
        customerID = ++customerCounter;
    }

    int getCustomerID() const {
        return customerID;
    }

    string getName() const {
        return name;
    }

    void createAccount() {
        Account newAcc;
        accounts.push_back(newAcc);
        cout << "New account #" << newAcc.getAccountNumber() << " created for " << name << endl;
    }

    Account* getAccountByNumber(int accNum) {
        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNum)
                return &acc;
        }
        cout << "Account #" << accNum << " not found for customer " << name << endl;
        return nullptr;
    }

    void displayCustomerAccounts() const {
        cout << "\nCustomer: " << name << " (ID: " << customerID << ")\n";
        for (const auto& acc : accounts) {
            acc.displayAccountInfo();
        }
    }
};
int Customer::customerCounter = 0;

int main() {
    Customer cust1("Ghulam");
    Customer cust2("Mustafa");

    cust1.createAccount();
    cust2.createAccount();

    Account* ghulamAcc = cust1.getAccountByNumber(1001);
    Account* mustafaAcc = cust2.getAccountByNumber(1002);

    if (ghulamAcc && mustafaAcc) {
        ghulamAcc->deposit(50000);
        ghulamAcc->withdraw(10000);
        ghulamAcc->transferTo(*mustafaAcc, 20000);

        cust1.displayCustomerAccounts();
        cust2.displayCustomerAccounts();
    }

    return 0;
}