// Last Edited 9/30/25, Sebastian Quirarte, Inheritance Lab,
//Side note, i've looked at the breakpoint assignemnt you had us do and realized how little my annotations are, so i will try and improve them to be more clearer

//Including all the basic stuff
#include <iostream>
#include <vector>
#include <memory>
#include <string>

// The base class
class BankAccount {
protected:
    std::string accountNumber;
    std::string accountHolderName;
    double balance;
    //Using a nested version as using just std::vector<Transaction> transactionHistory; left an error saying it was undefined. 
    //I will attempt to find a solution using what was provided but until then I will go with the nested function
    struct Transaction {
        std::string type; // "Deposit" or "Withdraw"
        double amount;
    };
    //then use the given private member variable now that it has something to go with storing the transactions over time
    std::vector<Transaction> transactionHistory;
    //They are here to build a log for transactions 
public:
    // Constructors
    BankAccount(std::string accNum = "0000", std::string holderName = "Unknown", double initialBalance = 0.0)
        : accountNumber(accNum), accountHolderName(holderName), balance(initialBalance >= 0 ? initialBalance : 0.0) {
    }

    // Virtual destructor for the polymorphic cleanup (made in public)
    virtual ~BankAccount() {
        std::cout << "BankAccount for " << accountHolderName << " destroyed.\n";
    }

    // Getters
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }

    // Deposit method
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            // Add to transaction history pushing it into transactionHistory and provides a message saying deposit (certain ammount)
            transactionHistory.push_back(Transaction{ "Deposit", amount });
            std::cout << "Deposit successful! New balance: $" << balance << std::endl;
        }
        else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    // Withdraw method (virtual for overriding)
    virtual bool withdraw(double amount) {
        //Checking for a valid amount <0
        if (amount <= 0) {
            std::cout << "Invalid withdrawal amount.\n";
            return false;
        }
        //Checks to see if you have enough balance to withdraw the money
        if (amount > balance) {
            std::cout << "Insufficient funds.\n";
            return false;
        }
        //This updates the balance to show the new one
        balance -= amount;
        // Add to transaction history as per instructions, it adds the withdraw to a history, it pushed it back into "transactionHistory" along with the line Withdraw, "then the amount"
        //It then shows the new balance with the provided comment
        transactionHistory.push_back(Transaction{ "Withdraw", amount });
        std::cout << "Withdrawal successful! New balance: $" << balance << std::endl;
        return true;
    }

    // Print account info (virtual so derived classes can extend it)
    virtual void printAccount() const {
        std::cout << "Account Number: " << accountNumber
            << ", Holder: " << accountHolderName
            << ", Balance: $" << balance << std::endl;
    }
    void printHistory() const {
        std::cout << "Transaction History for " << accountHolderName << ":\n";
        for (const auto& t : transactionHistory) {
            std::cout << t.type << ": $" << t.amount << "\n";
        }
    }
};

// Derived Class checking account
class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(std::string accNum, std::string holderName, double initialBalance, double fee)
        : BankAccount(accNum, holderName, initialBalance), transactionFee(fee) {
    }

    // Override withdraw with fee included
    bool withdraw(double amount) override {
        double total = amount + transactionFee;
        if (total > balance) {
            std::cout << "Insufficient funds (including transaction fee).\n";
            return false;
        }
        // Provides statement if successfull and shows balance after
        balance -= total;
        std::cout << "Withdrawal successful! $" << amount << " withdrawn + $" << transactionFee
            << " fee. New balance: $" << balance << std::endl;
        return true;
    }

    void printAccount() const override {
        BankAccount::printAccount();
        std::cout << "   [Checking Account | Fee: $" << transactionFee << "]\n";
    }
};

// Derived savings account 
class SavingsAccount : public BankAccount {
private: //privated per instructions
    double interestRate;

public:
    SavingsAccount(std::string accNum, std::string holderName, double initialBalance, double rate)
        : BankAccount(accNum, holderName, initialBalance), interestRate(rate) {
    }
    //provides interest
    void calculateInterest() {
        double interest = balance * interestRate;
        balance += interest;
        std::cout << "Interest added: $" << interest << ". New balance: $" << balance << std::endl;
    }
    // prints account with interest rate
    void printAccount() const override {
        BankAccount::printAccount();
        std::cout << "   [Savings Account | Interest Rate: " << (interestRate * 100) << "%]\n";
    }
};

// Menu Helpers 
void displayAccounts(const std::vector<std::unique_ptr<BankAccount>>& accounts) {
    if (accounts.empty()) {
        std::cout << "No accounts available sorry!\n";
        return;
    }
    for (const auto& acc : accounts) {
        acc->printAccount();
    }
}

// Main 
int main() {
    std::vector<std::unique_ptr<BankAccount>> accounts;
    int choice;

    do {
        //Choices that are provided 1-8
        std::cout << "\nBank System Menu:\n";
        std::cout << "Choose between 1-7 please!\n";
        std::cout << "1. Create Checking Account\n";
        std::cout << "2. Create Savings Account\n";
        std::cout << "3. View Accounts\n";
        std::cout << "4. Deposit Funds\n";
        std::cout << "5. Withdraw Funds\n";
        std::cout << "6. Add Interest (Savings Only)\n";
        std::cout << "7. View Transaction History\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        //Making the new account if 1 is entered
        if (choice == 1) {
            std::string accNum, name;
            double balance, fee;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            std::cout << "Enter account holder name: ";
            std::cin >> name;
            std::cout << "Enter initial balance: ";
            std::cin >> balance;
            std::cout << "Enter transaction fee: ";
            std::cin >> fee;
            accounts.push_back(std::make_unique<CheckingAccount>(accNum, name, balance, fee));
        }
        //finding the account if 2 is entered
        else if (choice == 2) {
            std::string accNum, name;
            double balance, rate;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            std::cout << "Enter account holder name: ";
            std::cin >> name;
            std::cout << "Enter initial balance: ";
            std::cin >> balance;
            std::cout << "Enter interest rate (ex: 0.05 for 5%): ";
            std::cin >> rate;
            accounts.push_back(std::make_unique<SavingsAccount>(accNum, name, balance, rate));
        }
        // displays all accounts if 3 is entered
        else if (choice == 3) {
            displayAccounts(accounts);
        }
        //depositing using the account info provided if 4 is entered
        else if (choice == 4) {
            std::string accNum;
            double amount;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            std::cout << "Enter deposit amount: ";
            std::cin >> amount;
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    acc->deposit(amount);
                }
            }
        }
        //taking money away from account with info provided if 5 is entered
        else if (choice == 5) {
            std::string accNum;
            double amount;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            std::cout << "Enter withdrawal amount: ";
            std::cin >> amount;
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    acc->withdraw(amount); // polymorphic call
                }
            }
        }
        //getting account if 6 is entered
        else if (choice == 6) {
            std::string accNum;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            for (auto& acc : accounts) {
                SavingsAccount* sa = dynamic_cast<SavingsAccount*>(acc.get());
                if (sa && sa->getAccountNumber() == accNum) {
                    sa->calculateInterest();
                }
            }
        }
        //finding transaction history if 7 is entered
        else if (choice == 7) {
            std::string accNum;
            std::cout << "Enter account number: ";
            std::cin >> accNum;
            //Looks throough any accounts in the syster
            for (auto& acc : accounts) {
                //checks to see if it matches with what was inputed
                if (acc->getAccountNumber() == accNum) {
                    //if they match it called the code below method
                    acc->printHistory();
                }
            }
        }
        //leaving the program if 8 is entered
        else if (choice == 8) {
            std::cout << "Exiting program, thank you for working with us!\n";
        }
        //if number isnt between 1-8
        else {
            std::cout << "Invalid choice. Try again please with a number 1-8 please\n";
        }
    } while (choice != 8);

    return 0;
}
