#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <typeinfo>

// This code is used to create a function that generates a random string of a specified length.
std::string generateRandomString(size_t length)
{
    std::string randomString;                     // String to store random string
    std::random_device rd;                        // Obtain a random number from hardware
    std::mt19937 gen(rd());                       // Seed the generator
    std::uniform_int_distribution<> dis(11, 1e6); // Define the range
    for (size_t i = 0; i < length; ++i)
    {
        randomString += static_cast<char>(dis(gen)); // Generate random string
    }
    return randomString;
}

// This code is used to create a custom exception that can be used when the financial
// calculations in the application fail. This will allow the application to continue
// running in the event of a financial calculation failure, rather than crashing.

class FinancialException : public std::exception
{
private:
    std::string message;

public:
    FinancialException(const std::string &errorMessage) : message(errorMessage) {}

    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

// Encrypt password with key using XOR
std::string encryptPassword(const std::string &password, const std::string &key)
{
    // Password is encrypted by XORing each character with a character in key
    // The key is repeated if it is shorter than the password
    std::string encryptedPassword = password;
    const size_t keyLength = key.length();

    for (size_t i = 0; i < password.length(); ++i)
    {
        encryptedPassword[i] ^= key[i % keyLength];
    }

    return encryptedPassword;
}

// This function decrypts the password contained in encryptedPassword using the key contained in key.
// The key must be the same key that was used to encrypt the password.

std::string decryptPassword(const std::string &encryptedPassword, const std::string &key)
{
    std::string decryptedPassword = encryptedPassword; // Password is decrypted by XORing each character with a character in key
    const size_t keyLength = key.length();

    for (size_t i = 0; i < encryptedPassword.length(); ++i)
    {
        decryptedPassword[i] ^= key[i % keyLength]; // Password is decrypted by XORing each character with a character in key
    }

    return decryptedPassword;
}

class InvalidPeriodException : public FinancialException
{
public:
    InvalidPeriodException() : FinancialException("Invalid period specified.") {}
};

class InvalidCategoryException : public FinancialException
{
public:
    InvalidCategoryException() : FinancialException("Invalid category specified.") {}
};

// This is a class that represents a date. It has three members: day, month, and year.
// It is used to store the date of a particular event.

class Date
{
private:
    int day;
    int month;
    int year;

public:
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    int getDay() const
    {
        return day;
    }

    int getMonth() const
    {
        return month;
    }

    int getYear() const
    {
        return year;
    }
    bool checkValidity() // checks if the date is valid or not
    {
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0)
            return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            return false;
        else if (month == 2)
        {
            if (year % 4 == 0 && day > 29)
                return false;
            else if (year % 4 != 0 && day > 28)
                return false;
        }
        else
            return true;
    }
};

// This code creates a Category class, which contains a string that stores the name of the category.
// The class has a constructor that takes a string as a parameter, and sets the name of the category to that string.
// It has a getter method that returns the name of the category.

class Category
{
private:
    std::string name;

public:
    Category(const std::string &name) : name(name) {}

    const std::string &getName() const
    {
        return name;
    }
};

// This code creates a base class for transactions.
// It includes a constructor that takes a double, a string, and a Date.
// It also includes a virtual destructor, a virtual function that returns the amount,
// a virtual function that returns the Date, and a virtual function that displays the transaction.
// It also includes a getter for the description.
// The class is abstract, so it cannot be instantiated.
// Members:
// amount: the amount of the transaction
// description: a description of the transaction
// date: the date of the transaction

class Transaction
{
protected:
    double amount;
    std::string description;
    Date date;

public:
    Transaction(double amount, const std::string &description, const Date &date)
        : amount(amount), description(description), date(date) {}

    virtual double getAmount() const
    {
        return amount;
    }

    virtual const Date &getDate() const
    {
        return date;
    }

    const std::string &getDescription() const
    {
        return description;
    }

    virtual void display() const = 0;

    virtual ~Transaction() {}
};

// This code is used to create income and expense objects, which are used in the Budget class.
// The Income class is a subclass of Transaction, and it has a constructor that takes a double, a string, and a Date.
// It also has a display method that displays the amount and description of the income.

class Income : public Transaction
{
public:
    Income(double amount, const std::string &description, const Date &date)
        : Transaction(amount, description, date) {}

    void display() const override
    {
        std::cout << "Income: +" << amount << " BDT - " << description << std::endl;
    }
};

// The Expense class is a subclass of Transaction, and it has a constructor that takes a double, a string, a Date, and a Category.

class Expense : public Transaction
{
private:
    Category category; // Category of the expense

public:
    Expense(double amount, const std::string &description, const Date &date, const Category &category)
        : Transaction(amount, description, date), category(category) {}

    const Category &getCategory() const
    {
        return category;
    }

    void display() const override
    {
        std::cout << "Expense: -" << amount << " BDT - " << description << std::endl;
    }
};

std::set<std::string> users;         // Set of usernames, used to check if a username is already taken
class User;                          // Forward declaration of User class
void saveUserData(const User &user); // Forward declaration of saveUserData function

// This code creates a User class, which contains a string that stores the username and a string that stores the password.
// It also contains a double that stores the balance, and a vector of Transaction pointers that stores the transactions.

class User
{
private:
    std::string id;
    std::string username;
    std::string password;
    double balance;
    std::vector<Transaction *> transactions; // Vector of Transaction pointers, used to store transactions

public:
    User(const std::string &username, const std::string &password) // Constructor that takes a username and a password
        : username(username), password(password), balance(0.0)
    {
        id = generateRandomString(username.length()); // Generate a random string of the same length as the username, used as a key to encrypt the password
    }

    void deepcopy(const User &user) // Deep copy function, used in copy constructor and assignment operator
    {
        id = user.id;
        username = user.username;
        password = user.password;
        balance = user.balance;
        for (const auto &transaction : user.transactions) // Copy transactions
        {
            // using dynamic cast to check if the transaction is an income or expense transaction
            const Income *incomeTransaction = dynamic_cast<const Income *>(transaction);    // Check if transaction is an income transaction
            const Expense *expenseTransaction = dynamic_cast<const Expense *>(transaction); // Check if transaction is an expense transaction
            if (incomeTransaction)
            {
                transactions.push_back(new Income(incomeTransaction->getAmount(), incomeTransaction->getDescription(), incomeTransaction->getDate()));
            }
            else if (expenseTransaction)
            {
                transactions.push_back(new Expense(expenseTransaction->getAmount(), expenseTransaction->getDescription(), expenseTransaction->getDate(), expenseTransaction->getCategory()));
            }
        }
    }

    User(const User &user) // Copy constructor
    {
        deepcopy(user); // Call deepcopy function
    }

    User &operator=(const User &user) // Assignment operator
    {
        if (this != &user)
        {
            for (const auto &transaction : transactions) // free memory allocated to transactions
            {
                delete transaction;
            }
            transactions.clear();
            deepcopy(user);
        }
        return *this; // Return a reference to the current object, so that the assignment operator can be chained
    }

    const std::string &getId() const // Getter for id
    {
        return id;
    }

    const std::string &getUsername() const // Getter for username
    {
        return username;
    }

    const std::string &getPassword() const // Getter for password, typically not used
    {
        return password;
    }

    const std::vector<Transaction *> &getTransactions() const // Getter for transactions
    {
        return transactions;
    }

    bool validatePassword(const std::string &inputPassword) const // Validate password
    {
        return password == inputPassword;
    }

    void changePassword(const std::string &newPassword) // Change password
    {
        password = newPassword;                             // Change password
        std::remove(("data/" + username + ".txt").c_str()); // Delete user file and save updated user data
        saveUserData(*this);                                // Save updated user data
    }

    double getBalance() const // Getter for balance
    {
        return balance;
    }

    void setBalance(double newBalance) // Setter for balance
    {
        balance = newBalance;
    }

    void addTransaction(Transaction *transaction) // Add new transaction to transactions vector
    {
        // using dynamic cast to check if the transaction is an income or expense transaction
        transactions.push_back(transaction);
        if (dynamic_cast<Income *>(transaction))
        {
            balance += transaction->getAmount(); // Add amount to balance
        }
        else if (dynamic_cast<Expense *>(transaction))
        {
            balance -= transaction->getAmount(); // Subtract amount from balance
        }
        saveUserData(*this); // Save updated user data
    }

    void removeTransaction(int index) // Remove transaction at index
    {
        if (index >= 0 && index < transactions.size())
        {
            Transaction *transaction = transactions[index]; // Get transaction at index
            if (dynamic_cast<Income *>(transaction))        // Check if transaction is an income transaction
            {
                balance -= transaction->getAmount(); // Subtract amount from balance
            }
            else if (dynamic_cast<Expense *>(transaction)) // Check if transaction is an expense transaction
            {
                balance += transaction->getAmount(); // Add amount to balance
            }
            transactions.erase(transactions.begin() + index); // Remove transaction from transactions vector
            delete transaction;                               // Free memory allocated to transaction
            saveUserData(*this);                              // Save updated user data
        }
    }

    double calculateOverallBalance() const // Calculate overall balance
    {
        return balance;
    }

    double calculateIncomeForPeriod(const Date &startDate, const Date &endDate) const // Calculate income for a period
    {
        double income = 0.0;
        for (const auto &transaction : transactions)
        {
            const Income *incomeTransaction = dynamic_cast<const Income *>(transaction); // Check if transaction is an income transaction
            if (incomeTransaction && isDateInRange(incomeTransaction->getDate(), startDate, endDate))
            {
                income += incomeTransaction->getAmount();
            }
        }
        return income;
    }

    double calculateExpensesForPeriod(const Date &startDate, const Date &endDate) const // Calculate expenses for a period
    {
        double expenses = 0.0;
        for (const auto &transaction : transactions) // Check if transaction is an expense transaction
        {
            const Expense *expenseTransaction = dynamic_cast<const Expense *>(transaction);
            if (expenseTransaction && isDateInRange(expenseTransaction->getDate(), startDate, endDate))
            {
                expenses += expenseTransaction->getAmount();
            }
        }
        return expenses;
    }

    double calculateNetSavingsForPeriod(const Date &startDate, const Date &endDate) const // Calculate net savings for a period
    {
        double income = calculateIncomeForPeriod(startDate, endDate);
        double expenses = calculateExpensesForPeriod(startDate, endDate);
        return income - expenses;
    }

    double calculateExpensesByCategory(const std::string &categoryName) const // Calculate expenses by category
    {
        double expenses = 0.0;
        for (const auto &transaction : transactions)
        {
            // Check if transaction is an expense transaction and if the category name matches
            const Expense *expenseTransaction = dynamic_cast<const Expense *>(transaction);
            if (expenseTransaction && expenseTransaction->getCategory().getName() == categoryName)
            {
                expenses += expenseTransaction->getAmount(); // Add amount to expenses
            }
        }
        return expenses;
    }

    // Generate reports

    // This function generates a summary report for a period specified by startDate and endDate.
    void generateSummaryReport(const Date &startDate, const Date &endDate) const
    {
        std::cout << "Summary Report from " << startDate.getDay() << "/" << startDate.getMonth() << "/" << startDate.getYear()
                  << " to " << endDate.getDay() << "/" << endDate.getMonth() << "/" << endDate.getYear() << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "Total Income: " << calculateIncomeForPeriod(startDate, endDate) << " BDT" << std::endl;
        std::cout << "Total Expenses: " << calculateExpensesForPeriod(startDate, endDate) << " BDT" << std::endl;
        std::cout << "Net Savings: " << calculateNetSavingsForPeriod(startDate, endDate) << " BDT" << std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }

    // This function generates a category report for a category specified by categoryName.
    void generateCategoryReport(const std::string &categoryName) const
    {
        std::cout << "Category Report: " << categoryName << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "Total Expenses in Category: " << calculateExpensesByCategory(categoryName) << " BDT" << std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }

    // This function generates a summary report for a period specified by startDate and endDate and saves it to a file.
    void createSummaryReportFile(const Date &startDate, const Date &endDate) const
    {
        std::string fileName = username + "_" + "summary_report_" + std::to_string(startDate.getDay()) + "_" + std::to_string(startDate.getMonth()) + "_" + std::to_string(startDate.getYear()) + "_" + std::to_string(endDate.getDay()) + "_" + std::to_string(endDate.getMonth()) + "_" + std::to_string(endDate.getYear()) + ".txt";
        std::ofstream file("reports/" + fileName); // Open file
        if (!file)
        {
            throw FinancialException("Error creating summary report file."); // Throw exception if file cannot be opened
        }

        // Write report to file
        file << "Summary Report from " << startDate.getDay() << "/" << startDate.getMonth() << "/" << startDate.getYear()
             << " to " << endDate.getDay() << "/" << endDate.getMonth() << "/" << endDate.getYear() << std::endl;
        file << "--------------------------------------" << std::endl;
        file << "Total Income: " << calculateIncomeForPeriod(startDate, endDate) << " BDT" << std::endl;
        file << "Total Expenses: " << calculateExpensesForPeriod(startDate, endDate) << " BDT" << std::endl;
        file << "Net Savings: " << calculateNetSavingsForPeriod(startDate, endDate) << " BDT" << std::endl;
        file << "--------------------------------------" << std::endl;

        file.close();
        std::cout << "Summary report file created successfully." << std::endl;
    }

    // This function generates a category report for a category specified by categoryName and saves it to a file.
    void createCategoryReportFile(const std::string &categoryName) const
    {
        std::string fileName = username + "_" + categoryName + "_report.txt";
        std::ofstream file("reports/" + fileName); // Open file
        if (!file)
        {
            throw FinancialException("Error creating category report file."); // Throw exception if file cannot be opened
        }

        // Write report to file
        file << "Category Report: " << categoryName << std::endl;
        file << "--------------------------------------" << std::endl;
        file << "Total Expenses in Category: " << calculateExpensesByCategory(categoryName) << " BDT" << std::endl;
        file << "--------------------------------------" << std::endl;

        file.close();
        std::cout << "Category report file created successfully." << std::endl;
    }

    ~User() // Destructor
    {
        // Free memory allocated to transactions
        for (const auto &transaction : transactions)
        {
            delete transaction;
        }
    }

private:
    bool isDateInRange(const Date &date, const Date &startDate, const Date &endDate) const // Check if date is in range
    {
        return date.getYear() >= startDate.getYear() && date.getYear() <= endDate.getYear() &&
               date.getMonth() >= startDate.getMonth() && date.getMonth() <= endDate.getMonth() &&
               date.getDay() >= startDate.getDay() && date.getDay() <= endDate.getDay();
    }
};

// This function saves the user data to a file.
void saveUserData(const User &user)
{
    std::string key = user.getId();                            // Get key
    std::ofstream file("data/" + user.getUsername() + ".txt"); // Open file associated with user
    if (!file)
    {
        throw FinancialException("Error saving user data: User file Cannot be opened."); // Throw exception if file cannot be opened
    }
    file.clear(); // Clear file contents in order to overwrite

    // Write user data to file
    file << key << std::endl;
    file << encryptPassword(user.getPassword(), key) << std::endl;
    file << user.getUsername() << std::endl;
    file << user.getBalance() << std::endl;
    for (const auto &transaction : user.getTransactions())
    {
        const Income *incomeTransaction = dynamic_cast<const Income *>(transaction);    // Check if transaction is an income transaction
        const Expense *expenseTransaction = dynamic_cast<const Expense *>(transaction); // Check if transaction is an expense transaction
        if (incomeTransaction)
        {
            // Write income transaction data to file
            file << "Income" << std::endl;
            file << incomeTransaction->getAmount() << std::endl;
            file << incomeTransaction->getDescription() << std::endl;
            file << incomeTransaction->getDate().getDay() << "/" << incomeTransaction->getDate().getMonth()
                 << "/" << incomeTransaction->getDate().getYear() << std::endl;
        }
        else if (expenseTransaction)
        {
            // Write expense transaction data to file
            file << "Expense" << std::endl;
            file << expenseTransaction->getAmount() << std::endl;
            file << expenseTransaction->getDescription() << std::endl;
            file << expenseTransaction->getDate().getDay() << "/" << expenseTransaction->getDate().getMonth()
                 << "/" << expenseTransaction->getDate().getYear() << std::endl;
            file << expenseTransaction->getCategory().getName() << std::endl;
        }
    }

    file.close();
}

// This function loads the user data from a file.
int loadUserData(const std::string &username, const std::string &password, User *&user)
{
    std::ifstream file("data/" + username + ".txt"); // Open file associated with user
    if (!file)
    {
        throw FinancialException("User data not found."); // Throw exception if file cannot be opened
    }
    std::string key;
    std::getline(file, key); // Get key from file
    std::string loadedPassword;
    std::getline(file, loadedPassword); // Get encrypted password from file
    std::string loadedUsername;
    std::getline(file, loadedUsername); // Get username from file
    if (loadedUsername != username)     // Check if username matches
    {
        return -1;
    }

    loadedPassword = decryptPassword(loadedPassword, key); // Decrypt password
    if (loadedPassword != password)                        // Check if password matches
    {
        return -2;
    }

    user = new User(username, password); // Create new user object

    // Load user data
    std::string transactionType;
    while (std::getline(file, transactionType))
    {
        if (transactionType == "Income")
        {
            std::string amountString;
            std::getline(file, amountString);
            double amount = std::stod(amountString);
            std::string description;
            std::getline(file, description);
            std::string dateString;
            std::getline(file, dateString);
            int day, month, year;
            sscanf(dateString.c_str(), "%d/%d/%d", &day, &month, &year);
            Date date(day, month, year);
            user->addTransaction(new Income(amount, description, date)); // Add income transaction
        }
        else if (transactionType == "Expense")
        {
            std::string amountString;
            std::getline(file, amountString);
            double amount = std::stod(amountString);
            std::string description;
            std::getline(file, description);
            std::string dateString;
            std::getline(file, dateString);
            int day, month, year;
            sscanf(dateString.c_str(), "%d/%d/%d", &day, &month, &year);
            Date date(day, month, year);
            std::string categoryName;
            std::getline(file, categoryName);
            Category category(categoryName);
            user->addTransaction(new Expense(amount, description, date, category)); // Add expense transaction
        }
    }

    file.close();
    return 0;
}

// This function registers a new user.
User *registerUser(const std::string &username, const std::string &password)
{
    // Check if user already exists
    if (users.count(username))
    {
        throw FinancialException("User already exists.");
    }
    // Add user to the set of users
    users.insert(username);
    std::ofstream file("data/users.txt", std::ios::app);
    if (!file)
    {
        throw FinancialException("Error saving user data: User file Cannot be opened.");
    }
    file << username << std::endl;
    // Create a new user
    User user(username, password);
    // Save user data
    saveUserData(user);
    std::cout << "User " << username << " registered successfully." << std::endl;
    return new User(user);
}

// This function deletes a user.
User *deleteUser(const std::string &username, const std::string &password)
{
    // Check if user exists
    auto it = users.find(username);
    if (it == users.end())
    {
        throw FinancialException("User does not exist.");
    }
    // Remove user from the set of users
    users.erase(it);
    // Delete user file
    std::remove(("data/" + username + ".txt").c_str());
    // remove user from users.txt
    std::ifstream file("data/users.txt");             // Open users.txt
    std::ofstream temp("data/temp.txt");              // Create temp.txt
    for (std::string line; std::getline(file, line);) // Read users.txt line by line and write to temp.txt, except for the line containing the username
    {
        if (line != username)
        {
            temp << line << std::endl;
        }
    }
    file.close();
    temp.close();
    std::remove("data/users.txt");                  // Delete users.txt
    std::rename("data/temp.txt", "data/users.txt"); // Rename temp.txt to users.txt, effectively removing the user from users.txt
    return nullptr;
}

void login(const std::string &username, const std::string &password, User *&user)
{
    // Check if user exists
    auto it = users.find(username);
    if (it == users.end())
    {
        throw FinancialException("User does not exist."); // Throw exception if user does not exist
    }
    // Load user data, if password is wrong add option to change password,try again or exit
    int status = loadUserData(username, password, user);
    if (status == -1)
    {
        throw FinancialException("Invalid username."); // Throw exception if username is wrong
    }
    else if (status == -2)
    {
        throw FinancialException("Invalid password."); // Throw exception if password is wrong
    }
    std::cout << "User " << username << " logged in successfully." << std::endl;
}

// User Interface Functions

void mainMenu()
{
    std::cout << "Welcome to Your Personal Finance Manager!" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "               1. Register" << std::endl;
    std::cout << "               2. Login" << std::endl;
    std::cout << "               3. About" << std::endl;
    std::cout << "               4. Help" << std::endl;
    std::cout << "               5. Exit" << std::endl;
}

void userMenu()
{
    std::cout << "   __User Menu__  " << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "1. Add Income" << std::endl;
    std::cout << "2. Add Expense" << std::endl;
    std::cout << "3. Remove Income/Expense" << std::endl;
    std::cout << "4. View Transactions" << std::endl;
    std::cout << "5. View Balance" << std::endl;
    std::cout << "6. Change Password" << std::endl;
    std::cout << "7. Generate Summary Report" << std::endl;
    std::cout << "8. Generate Category Report" << std::endl;
    std::cout << "9. Logout" << std::endl;
    std::cout << "0. Delete Account" << std::endl;
    std::cout << "#. Exit" << std::endl;
}

// Clears the terminal screen.

void clearTerminal()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-like systems
#endif
}

// This function is used to get a string from the user.
void registerProcess(User *&user)
{
    std::string username, password, confirmPassword;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    std::cout << "Confirm password: ";
    std::getline(std::cin, confirmPassword);
    if (password != confirmPassword)
    {
        throw FinancialException("Passwords do not match."); // Throw exception if passwords do not match
    }
    else
        registerUser(username, password); // Register user
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// This function is used to get a string from the user.
void loginprocess(User *&user)
{
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    login(username, password, user); // Login
    std::cout << "Press enter to continue...";
    std::cin.ignore();
}

// Shows the about information.
void about()
{
    std::cout << "This is a personal finance manager application which can be used to manage your income and expenses." << std::endl;
    std::cout << "It can also be used to generate summary reports and category reports." << std::endl;
    std::cout << "It is a command line application written in C++." << std::endl
              << std::endl;
    std::cout << "             Information about the developer: " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "                       Raufun Ahsan" << std::endl;
    std::cout << "      Department of Computer Science and Engineering" << std::endl;
    std::cout << " Khulna University of Engineering and Technology, Khulna" << std::endl;
    std::cout << "                    Roll: 2007030" << std::endl;
    std::cout << "         Email: ahsan2007030@stud.kuet.ac.bd" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// Shows the help information.
void help()
{
    std::cout << "                               Help" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "           On each menu, enter your choice of operation." << std::endl;
    std::cout << "          You can see the list of operations on the menu." << std::endl;
    std::cout << "  You can enter your financial data according to the instructions." << std::endl;
    std::cout << "       You can also generate reports according to your needs." << std::endl;
    std::cout << "      If you wish to save the report to a file, you can do so." << std::endl;
    std::cout << "             The files are saved in the reports folder." << std::endl;
    std::cout << std::endl;
    std::cout << "Press enter to continue...";
    std::cin.ignore();
}

// Function to add income to the user logged in
void addIncome(User *&user)
{
    std::string amountString, description;
    double amount;
    std::cout << "Enter amount: ";
    std::getline(std::cin, amountString);
    amount = std::stod(amountString);
    std::cout << "Enter description: ";
    std::getline(std::cin, description);
    std::cout << "Enter date (dd/mm/yyyy): ";
    std::string dateString;
    std::getline(std::cin, dateString);
    int day, month, year;
    sscanf(dateString.c_str(), "%d/%d/%d", &day, &month, &year);
    Date date(day, month, year);
    if (!date.checkValidity()) // Check if date is valid
    {
        std::cout << "Invalid date!" << std::endl; // Throw exception if date is invalid
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.ignore();
        return;
    }
    user->addTransaction(new Income(amount, description, date)); // Add income transaction
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// Function to add expense to the user logged in
void addExpense(User *&user)
{
    std::string amountString, description, categoryName;
    double amount;
    std::cout << "Enter amount: ";
    std::getline(std::cin, amountString);
    amount = std::stod(amountString);
    std::cout << "Enter description: ";
    std::getline(std::cin, description);
    std::cout << "Enter date (dd/mm/yyyy): ";
    std::string dateString;
    std::getline(std::cin, dateString);
    int day, month, year;
    sscanf(dateString.c_str(), "%d/%d/%d", &day, &month, &year);
    Date date(day, month, year);
    if (!date.checkValidity()) // Check if date is valid
    {
        std::cout << "Invalid date!" << std::endl; // Throw exception if date is invalid
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.ignore();
        return;
    }
    std::cout << "Enter category: ";
    std::getline(std::cin, categoryName);
    Category category(categoryName);
    user->addTransaction(new Expense(amount, description, date, category)); // Add expense transaction
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// Function to view transactions of the user logged in
void removeIncome(User *&user)
{
    std::string indexString;
    int index;
    std::cout << "Enter index of income to remove: ";
    std::getline(std::cin, indexString);
    index = std::stoi(indexString);
    user->removeTransaction(index);
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// Function to view transactions of the user logged in
void generateSummaryReport(User *&user)
{
    std::cout << "Enter start date (dd/mm/yyyy): ";
    std::string startDateString;
    std::getline(std::cin, startDateString);
    int day, month, year;
    sscanf(startDateString.c_str(), "%d/%d/%d", &day, &month, &year);
    Date startDate(day, month, year);
    if (!startDate.checkValidity()) // Check if date is valid
    {
        std::cout << "Invalid date!" << std::endl;
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.ignore();
        return;
    }
    std::cout << "Enter end date (dd/mm/yyyy): ";
    std::string endDateString;
    std::getline(std::cin, endDateString);
    sscanf(endDateString.c_str(), "%d/%d/%d", &day, &month, &year);
    Date endDate(day, month, year);
    if (!endDate.checkValidity()) // Check if date is valid
    {
        std::cout << "Invalid date!" << std::endl;
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.ignore();
        return;
    }
    user->generateSummaryReport(startDate, endDate);
    std::cout << "Do you want to save the report to a file? (y/n): ";
    char choice;
    std::cin >> choice; // Check if user wants to save the report to a file
    if (choice == 'y' || choice == 'Y')
    {
        try
        {
            user->createSummaryReportFile(startDate, endDate);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.ignore();
}

// Function to view transactions of the user logged in
void generateCategoryReport(User *&user)
{
    std::string categoryName;
    std::cout << "Enter category name: ";
    std::getline(std::cin, categoryName);
    user->generateCategoryReport(categoryName);
    std::cout << "Do you want to save the report to a file? (y/n): ";
    char choice;
    std::cin >> choice; // Check if user wants to save the report to a file
    if (choice == 'y' || choice == 'Y')
    {
        try
        {
            user->createCategoryReportFile(categoryName);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << "Press enter to continue...";
    std::cin.ignore();
}

// Driver function

int main()
{
    users.clear();                                    // Clear set of users
    std::ifstream file("data/users.txt");             // Open users.txt
    for (std::string line; std::getline(file, line);) // Read users.txt line by line and add each line to the set of users
    {
        users.insert(line);
    }
    file.close();
    User *user = nullptr; // Pointer to user object. If user is logged in, this pointer points to the user object. Otherwise, it is nullptr.
    std::string input;    // String to store user input, used for menu choices
    int choice;
    try
    {
        // A do while loop is used to show the menu and get user input
        // The loop continues until the user chooses to exit
        do
        {
            clearTerminal();
            try
            {
                if (!user) // Check if user is logged in. If not, show main menu
                {
                    mainMenu();
                    std::cout << "Enter your choice: ";
                    std::getline(std::cin, input); // Get user input
                    if (input.length() > 1)        // Check if input is valid
                    {
                        std::cout << "Invalid choice!" << std::endl;
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        continue;
                    }
                    choice = std::stoi(input);
                    switch (choice) // Switch case to handle user input
                    {
                    case 1: // Register
                    {
                        clearTerminal();
                        std::cout << "      New User Regisration" << std::endl;
                        std::cout << "--------------------------------" << std::endl;
                        try
                        {
                            registerProcess(user);
                        }
                        catch (const FinancialException &e)
                        {
                            std::cout << e.what() << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case 2: // Login
                    {
                        clearTerminal();
                        std::cout << "      User Login" << std::endl;
                        std::cout << "-------------------------" << std::endl;
                        try
                        {
                            loginprocess(user);
                        }
                        catch (const FinancialException &e)
                        {
                            std::cout << e.what() << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case 3: // About
                    {
                        clearTerminal();
                        about();
                        break;
                    }
                    case 4: // Help
                    {
                        clearTerminal();
                        help();
                        break;
                    }
                    case 5: // Exit
                    {
                        clearTerminal();
                        std::cout << "Are you sure you want to exit? (y/n): ";
                        char choice;
                        std::cin >> choice;
                        if (choice == 'y' || choice == 'Y')
                        {
                            if (user)
                            {
                                saveUserData(*user);
                            }
                            std::cout << "Exiting..." << std::endl;
                            return 0;
                        }
                        break;
                    }
                    default:
                    {
                        std::cout << "Invalid choice!" << std::endl;
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        break;
                    }
                    }
                }
                else // If user is logged in, show user menu
                {
                    userMenu();
                    std::cout << "Enter your choice: ";
                    std::getline(std::cin, input);
                    if (input.length() > 1) // Check if input is valid
                    {
                        std::cout << "Invalid choice!" << std::endl;
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        continue;
                    }
                    choice = input[0];
                    switch (choice)
                    {
                    case '1': // Add income
                    {
                        clearTerminal();
                        std::cout << "     Add Income" << std::endl;
                        std::cout << "--------------------" << std::endl;
                        try
                        {
                            addIncome(user);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case '2': // Add expense
                    {
                        clearTerminal();
                        std::cout << "     Add Expense" << std::endl;
                        std::cout << "--------------------" << std::endl;
                        try
                        {
                            addExpense(user);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case '3': // Remove income/expense
                    {
                        clearTerminal();
                        std::cout << "     Remove Income/Expense" << std::endl;
                        std::cout << "-------------------------------" << std::endl;
                        try
                        {
                            removeIncome(user);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case '4': // View transactions
                    {
                        clearTerminal();
                        std::cout << "            Transactions:" << std::endl;
                        std::cout << "--------------------------------------" << std::endl;
                        std::vector<Transaction *> transactions = user->getTransactions();
                        for (const auto &transaction : transactions)
                        {
                            transaction->display();
                            std::cout << "--------------------------------------" << std::endl;
                        }
                        std::cout << "Press enter to continue...";
                        std::cin.ignore();
                        break;
                    }
                    case '5': // View balance
                    {
                        clearTerminal();
                        std::cout << "Balance: " << user->getBalance() << " BDT" << std::endl;
                        std::cout << "Press enter to continue...";
                        std::cin.ignore();
                        break;
                    }
                    case '6': // Change password
                    {
                        clearTerminal();
                        try
                        {
                            std::cout << "     Change Password" << std::endl;
                            std::cout << "---------------------------" << std::endl;
                            std::string newPassword, confirmPassword;
                            std::cout << "Enter new password: ";
                            std::getline(std::cin, newPassword);
                            std::cout << "Confirm new password: ";
                            std::getline(std::cin, confirmPassword);
                            if (newPassword != confirmPassword)
                            {
                                throw FinancialException("Passwords do not match.");
                            }
                            else
                            {
                                user->changePassword(newPassword);
                                std::cout << "Password changed successfully." << std::endl;
                            }
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl; // Throw exception if passwords do not match
                        }

                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        break;
                    }
                    case '7': // Generate summary report
                    {
                        clearTerminal();
                        std::cout << "            Summary Report" << std::endl;
                        std::cout << "--------------------------------------" << std::endl;
                        try
                        {
                            generateSummaryReport(user);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl; // Throw exception if date is invalid
                        }
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        break;
                    }
                    case '8': // Generate category report
                    {
                        clearTerminal();
                        std::cout << "            Category Report" << std::endl;
                        std::cout << "--------------------------------------" << std::endl;
                        try
                        {
                            generateCategoryReport(user);
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl; // Throw exception if date is invalid
                        }
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        break;
                    }
                    case '9': // Logout
                    {
                        clearTerminal();
                        std::cout << "Are you sure you want to logout? (y/n): ";
                        char choice;
                        std::cin >> choice;
                        if (choice == 'y' || choice == 'Y') // Check if user wants to logout
                        {
                            saveUserData(*user); // Save user data to file before logging out
                            delete user;         // Delete user object
                            user = nullptr;      // Set user pointer to nullptr
                            std::cout << "Logged out successfully." << std::endl;
                            std::cout << "Press enter to continue..." << std::endl;
                            std::cin.ignore();
                        }
                        break;
                    }
                    case '0': // Delete account
                    {
                        clearTerminal();
                        try
                        {
                            std::cout << "Are you sure you want to delete your account? (y/n): ";
                            char choice;
                            std::cin >> choice;
                            if (choice == 'y' || choice == 'Y') // Check if user wants to delete account
                            {
                                user = deleteUser(user->getUsername(), user->getPassword()); // Delete user
                                std::cout << "Account deleted successfully." << std::endl;
                                std::cout << "Press enter to continue...";
                                std::cin.ignore();
                            }
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << e.what() << std::endl; // Throw exception if user does not exist
                            std::cout << "Press enter to continue...";
                            std::cin.ignore();
                        }
                        break;
                    }
                    case '#': // Exit
                    {
                        clearTerminal();
                        std::cout << "Are you sure you want to exit? (y/n): ";
                        char choice;
                        std::cin >> choice; // Check if user wants to exit
                        if (choice == 'y' || choice == 'Y')
                        {
                            saveUserData(*user); // Save user data to file before exiting
                            delete user;         // Delete user object
                            user = nullptr;      // Set user pointer to nullptr
                            std::cout << "Exiting..." << std::endl;
                            return 0;
                        }
                        break;
                    }
                    default:
                    {
                        std::cout << "Invalid choice!" << std::endl; // Throw exception if input is invalid
                        std::cout << "Press enter to continue..." << std::endl;
                        std::cin.ignore();
                        break;
                    }
                    }
                }
            }
            catch (const FinancialException &exception)
            {
                std::cout << exception.what() << std::endl; // Throw exception if any exception occurs
                std::cout << "Press enter to continue..." << std::endl;
                std::cin.ignore();
            }
            catch (...)
            {
                std::cout << "Unknown exception occurred." << std::endl; // Throw exception if any exception occurs
                std::cout << "Press enter to continue..." << std::endl;
                std::cin.ignore();
            }
        } while (1);
    }
    catch (const FinancialException &exception)
    {
        std::cout << exception.what() << std::endl; // Throw exception if any exception occurs
    }
    catch (...)
    {
        std::cout << "Unknown exception occurred." << std::endl; // Throw exception if any exception occurs
    }
    return 0;
}