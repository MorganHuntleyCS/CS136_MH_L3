// Morgan Huntley L3 10/4/25
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const string INPUT_FILE = "requests.txt";
const string OUTPUT_FILE = "accounts.txt";
const string ERROR_FILE = "errorfile.txt";
const string LOG_FILE = "transactions.txt";
const int STACK_SIZE = 100;
const int SSN_LENGTH = 10;
const int MIN_NAME_LENGTH = 2;
const int MIN_USERNAME_LENGTH = 4;
const int MIN_MAILSERVER_LENGTH = 4;
const double AVAILABLE_BALANCE = 0.0;
const double STUDENT_BONUS = 150.0;
const double REGULAR_BONUS = 100.0;
const int ACCOUNT_PREFIX_DIGITS = 6;
const int ACCOUNT_SUFFIX_DIGITS = 2;

enum MenuOption {
    PROCESS = 1,
    PRINT_CREATED,
    PRINT_INVALID,
    PRINT_LOG,
    QUIT
};

class CheckingAccount;
class Stack;
void logTransaction(const string& message);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
bool validateSSN(const string& ssn);
bool validateName(const string& name);
bool validateEmail(const string& email);
string genAccNum(int sequentialNum);
bool accountType(const string& email);
void professAccounts(Stack& accountStack);
void printCreatedAccounts(const Stack& accountStack);
void printInvalidRecords();
void printLogFile();
void writeAccounts(Stack& accountStack);
void displayMenu();
void clearInput();

class CheckingAccount {
private:
    string accountNumber;
    string ssn;
    string firstName;
    string lastName;
    string email;
    double availableBalance;
    double presentBalance;

    bool isValidData(const string& accNum, const string& s, const string& fn,
        const string& ln, const string& em, double avail, double present) const;

public:
    CheckingAccount();

    void setAll(const string& accNum, const string& s, const string& fn,
        const string& ln, const string& em, double avail, double present);

    CheckingAccount(const string& accNum, const string& s, const string& fn,
        const string& ln, const string& em, double avail, double present);

    string getAccountNumber() const;
    string getSSN() const;
    string getFirstName() const;
    string getLastName() const;
    string getEmail() const;
    double getAvailableBalance() const;
    double getPresentBalance() const;
    bool isEmpty() const;
};

class Stack {
private:
    CheckingAccount data[STACK_SIZE];
    int topIndex;

public:
    Stack();

    bool push(const CheckingAccount& account);
    bool pop(CheckingAccount& account);
    bool peek(CheckingAccount& account) const;

    bool isEmpty() const;
    int size() const;
    CheckingAccount getAt(int index) const;
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Stack accountStack;

    ofstream clearLog(LOG_FILE.c_str()); // Worked on most of my tests, but on one ocasion it didn't clear? Not sure what the error was. 
    clearLog.close();

    logTransaction("SYSTEM: Bank Account Processing System started");

    int choice = 0;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Please enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice) {
        case PROCESS:
            professAccounts(accountStack);
            break;
        case PRINT_CREATED:
            printCreatedAccounts(accountStack);
            break;
        case PRINT_INVALID:
            printInvalidRecords();
            break;
        case PRINT_LOG:
            printLogFile();
            break;
        case QUIT:
            writeAccounts(accountStack);
            cout << "\nThank you for using the Bank Account Processing System.\n"
                "All accounts have been saved. Goodbye!\n";
            logTransaction("SYSTEM: System shutdown");
            running = false;
            break;
        default:
            clearInput();
            cout << "Invalid choice. Please enter a number between 1 and 5.\n";
            break;
        }
    }

    return 0;
}

CheckingAccount::CheckingAccount() {
    accountNumber = "";
    ssn = "";
    firstName = "";
    lastName = "";
    email = "";
    availableBalance = 0.0;
    presentBalance = 0.0;
}

bool CheckingAccount::isValidData(const string& accNum, const string& s,
    const string& fn, const string& ln,
    const string& em, double avail, double present) const {
    bool valid = true;

    if (accNum.empty() || s.empty() || fn.empty() || ln.empty() || em.empty()) {
        valid = false;
    }
    if (avail < 0 || present < 0) {
        valid = false;
    }

    return valid;
}

void CheckingAccount::setAll(const string& accNum, const string& s,
    const string& fn, const string& ln,
    const string& em, double avail, double present) {
    if (!isValidData(accNum, s, fn, ln, em, avail, present)) {
        return;
    }
    accountNumber = accNum;
    ssn = s;
    firstName = fn;
    lastName = ln;
    email = em;
    availableBalance = avail;
    presentBalance = present;
}

CheckingAccount::CheckingAccount(const string& accNum, const string& s,
    const string& fn, const string& ln,
    const string& em, double avail, double present) {
    accountNumber = "";
    ssn = "";
    firstName = "";
    lastName = "";
    email = "";
    availableBalance = 0.0;
    presentBalance = 0.0;
    setAll(accNum, s, fn, ln, em, avail, present);
}

string CheckingAccount::getAccountNumber() const {
    return accountNumber;
}

string CheckingAccount::getSSN() const {
    return ssn;
}

string CheckingAccount::getFirstName() const {
    return firstName;
}

string CheckingAccount::getLastName() const {
    return lastName;
}

string CheckingAccount::getEmail() const {
    return email;
}

double CheckingAccount::getAvailableBalance() const {
    return availableBalance;
}

double CheckingAccount::getPresentBalance() const {
    return presentBalance;
}

bool CheckingAccount::isEmpty() const {
    return accountNumber.empty();
}

Stack::Stack() {
    topIndex = -1;
}

bool Stack::push(const CheckingAccount& account) {
    bool success = false;

    if (topIndex < STACK_SIZE - 1) {
        topIndex++;
        data[topIndex] = account;
        success = true;
    }

    return success;
}

bool Stack::pop(CheckingAccount& account) {
    bool success = false;

    if (topIndex >= 0) {
        account = data[topIndex];
        data[topIndex] = CheckingAccount();
        topIndex--;
        success = true;
    }

    return success;
}

bool Stack::peek(CheckingAccount& account) const {
    bool success = false;

    if (topIndex >= 0) {
        account = data[topIndex];
        success = true;
    }

    return success;
}

bool Stack::isEmpty() const {
    return topIndex < 0;
}

int Stack::size() const {
    return topIndex + 1;
}

CheckingAccount Stack::getAt(int index) const {
    CheckingAccount account;

    if (index >= 0 && index <= topIndex) {
        account = data[index];
    }

    return account;
}

void logTransaction(const string& message) {
    ofstream logFile(LOG_FILE.c_str(), ios::app);
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.close();
    }
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool validateSSN(const string& ssn) {
    bool valid = true;

    if (ssn.length() != SSN_LENGTH) {
        valid = false;
    }
    else {
        for (int i = 0; i < ssn.length() && valid; i++) {
            if (!isDigit(ssn[i])) {
                valid = false;
            }
        }
    }

    return valid;
}

bool validateName(const string& name) {
    bool valid = true;

    if (name.length() < MIN_NAME_LENGTH) {
        valid = false;
    }
    else {
        for (int i = 0; i < name.length() && valid; i++) {
            if (!isAlpha(name[i])) {
                valid = false;
            }
        }
    }

    return valid;
}

bool validateEmail(const string& email) {
    int atPos = -1;
    int dotPos = -1;
    bool valid = true;

    for (int i = 0; i < email.length() && valid; i++) {
        if (email[i] == '@') {
            if (atPos != -1) {
                valid = false;
            }
            atPos = i;
        }
    }

    if (atPos == -1 || atPos < MIN_USERNAME_LENGTH) {
        valid = false;
    }

    if (valid) {
        for (int i = 0; i < atPos && valid; i++) {
            if (!isAlphaNumeric(email[i]) && email[i] != '.' && email[i] != '_') {
                valid = false;
            }
        }
    }

    if (valid) {
        for (int i = atPos + 1; i < email.length(); i++) {
            if (email[i] == '.') {
                dotPos = i;
            }
        }

        if (dotPos == -1 || dotPos <= atPos + MIN_MAILSERVER_LENGTH) {
            valid = false;
        }
    }

    if (valid) {
        for (int i = atPos + 1; i < dotPos && valid; i++) {
            if (!isAlpha(email[i])) {
                valid = false;
            }
        }
    }

    if (valid) {
        string domain = "";
        for (int i = dotPos + 1; i < email.length(); i++) {
            domain += email[i];
        }

        if (domain != "com" && domain != "edu") {
            valid = false;
        }
    }

    return valid;
}

string genAccNum(int sequentialNum) {
    string accNum = "";

    for (int i = 0; i < ACCOUNT_PREFIX_DIGITS; i++) {
        int digit = rand() % 10;
        accNum += static_cast<char>('0' + digit);
    }

    if (sequentialNum < 10) {
        accNum += "0";
    }
    accNum += to_string(sequentialNum);

    return accNum;
}

bool accountType(const string& email) {
    int dotPos = -1;
    bool isEdu = false;

    for (int i = email.length() - 1; i >= 0; i--) {
        if (email[i] == '.') {
            dotPos = i;
            break;
        }
    }

    if (dotPos != -1) {
        string domain = "";
        for (int i = dotPos + 1; i < email.length(); i++) {
            domain += email[i];
        }

        if (domain == "edu") {
            isEdu = true;
        }
    }

    return isEdu;
}

void professAccounts(Stack& accountStack) {
    ifstream inputFile(INPUT_FILE.c_str());
    ofstream errorFile(ERROR_FILE.c_str());

    if (!inputFile.is_open()) {
        cout << "\nError: Couldn't open input the file '" << INPUT_FILE << "'.\n";
        logTransaction("ERROR: Couldn't open the input file");
        return;
    }

    int totalRecords = 0;
    int validAccounts = 0;
    int invalidRecords = 0;
    int sequentialNum = 1;

    string ssn, firstName, lastName, email;

    while (inputFile >> ssn >> firstName >> lastName >> email) {
        totalRecords++;

        bool valid = true;
        string errorMsg = "";

        if (!validateSSN(ssn)) {
            valid = false;
            errorMsg += "Invalid SSN (must be 10 digits). ";
        }

        if (!validateName(firstName)) {
            valid = false;
            errorMsg += "Invalid first name (min 2 chars). ";
        }

        if (!validateName(lastName)) {
            valid = false;
            errorMsg += "Invalid last name (min 2 chrs). ";
        }

        if (!validateEmail(email)) {
            valid = false;
            errorMsg += "Invalid email format. ";
        }

        if (valid) {
            string accountNum = genAccNum(sequentialNum);
            sequentialNum++;

            double bonus = accountType(email) ? STUDENT_BONUS : REGULAR_BONUS;

            CheckingAccount newAccount(accountNum, ssn, firstName, lastName,
                email, AVAILABLE_BALANCE, bonus);

            if (accountStack.push(newAccount)) {
                validAccounts++;
                logTransaction("SUCCESS: Account " + accountNum + " created for " + firstName + " " + lastName);
            }
            else {
                cout << "\nWarning: Stack is full. Cannot process more accounts.\n";
                logTransaction("ERROR: Stack is full, cannot add more accounts");
                break;
            }
        }
        else {
            invalidRecords++;
            errorFile << ssn << " " << firstName << " " << lastName << " " << email << " - " << errorMsg << "\n";
            logTransaction("INVALID: " + ssn + " " + firstName + " " + lastName + " - " + errorMsg);
        }
    }

    inputFile.close();
    errorFile.close();

    cout << "\n=== Processing Summary ===\n"
        "Total records processed: " << totalRecords << "\n"
        "Valid accounts created: " << validAccounts << "\n"
        "Invalid records: " << invalidRecords << "\n";

    logTransaction("SUMMARY: Processed=" + to_string(totalRecords) + " Valid=" + to_string(validAccounts) + " Invalid=" + to_string(invalidRecords));
}

void printCreatedAccounts(const Stack& accountStack) {
    if (accountStack.isEmpty()) {
        cout << "\nNo accounts to display.\n";
        return;
    }

    cout << "\n\n=== Successfully Created Accounts ===\n";
    cout << left << setw(12) << "Account#" << setw(12) << "SSN" << setw(15) << "First Name" << setw(15) << "Last Name" << setw(30) << "Email" << right << setw(12) << "Available" << setw(12) << "Present" << "\n";
    cout << string(108, '-') << "\n";

    for (int i = 0; i < accountStack.size(); i++) {
        CheckingAccount acc = accountStack.getAt(i);
        cout << left << setw(12) << acc.getAccountNumber() << setw(12) << acc.getSSN() << setw(15) << acc.getFirstName() << setw(15) << acc.getLastName()
            << setw(30) << acc.getEmail() << right << setw(12) << fixed << setprecision(2) << acc.getAvailableBalance() << setw(12) << fixed << setprecision(2) << acc.getPresentBalance() << "\n";
    }

    logTransaction("INFO: Displayed " + to_string(accountStack.size()) + " created accounts");
}

void printInvalidRecords() {
    ifstream errorFile(ERROR_FILE.c_str());

    if (!errorFile.is_open()) {
        cout << "\nNo invalid record file found.\n";
        return;
    }

    cout << "\n\n=== Invalid Records ===\n";
    string line;
    int count = 0;
    while (getline(errorFile, line)) {
        cout << line << "\n";
        count++;
    }

    if (count == 0) {
        cout << "No invalid records.\n";
    }

    errorFile.close();
    logTransaction("INFO: Displayed " + to_string(count) + " invalid records");
}

void printLogFile() {
    ifstream logFile(LOG_FILE.c_str());

    if (!logFile.is_open()) {
        cout << "\nNo log file found.\n";
        return;
    }

    cout << "\n\n=== Transaction Log ===\n";
    string line;
    while (getline(logFile, line)) {
        cout << line << "\n";
    }

    logFile.close();
}

void writeAccounts(Stack& accountStack) {
    if (accountStack.isEmpty()) {
        cout << "\nNo accounts to write.\n";
        return;
    }

    ofstream outputFile(OUTPUT_FILE.c_str());

    if (!outputFile.is_open()) {
        cout << "\nError: Couldn't open output file '" << OUTPUT_FILE << "'.\n";
        logTransaction("ERROR: Couldn't open output file");
        return;
    }

    outputFile << left << setw(12) << "Account#" << setw(12) << "SSN" << setw(15) << "First Name" << setw(15) << "Last Name" << setw(30) << "Email" << right << setw(12) << "Available" << setw(12) << "Present" << "\n";
    outputFile << string(108, '-') << "\n";

    int count = 0;
    CheckingAccount acc;

    while (accountStack.pop(acc)) {
        outputFile << left << setw(12) << acc.getAccountNumber() << setw(12) << acc.getSSN() << setw(15) << acc.getFirstName() << setw(15) << acc.getLastName()
            << setw(30) << acc.getEmail() << right << setw(12) << fixed << setprecision(2) << acc.getAvailableBalance() << setw(12) << fixed << setprecision(2) << acc.getPresentBalance() << "\n";
        count++;
    }

    outputFile.close();

    cout << "\n" << count << " accounts written to " << OUTPUT_FILE << "\n";
    logTransaction("INFO: Wrote " + to_string(count) + " accounts to output file");
}

void displayMenu() {
    cout << "\n\n======================================\n"
        "  Bank Account Processing System\n"
        "======================================\n"
        "1 Process new checking account requests\n"
        "2 Print all successfully created accounts\n"
        "3 Print all invalid records\n"
        "4 Print transaction log\n"
        "5 Quit and save accounts to file\n"
        "======================================\n"
        "Enter choice: ";
}

void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

/*
Inputs
1234567890 Mary Lee lee35@lapc.edu
9876543210 John Smith jsmith@gmail.com
123456789 Bob Jones bjones123@company.com
1234567890 A B short@test.edu
1234567890 Sarah Johnson sarah.j@college.edu
9999999999 Mike Davis mdavis_1@work.com

TEST RESULTS:

======================================
  Bank Account Processing System
======================================
1 Process new checking account requests
2 Print all successfully created accounts
3 Print all invalid records
4 Print transaction log
5 Quit and save accounts to file
======================================
Enter choice: 1

=== Processing Summary ===
Total records processed: 7
Valid accounts created: 5
Invalid records: 2

Enter choice: 2

=== Successfully Created Accounts ===
Account#    SSN         First Name     Last Name      Email                            Available     Present
------------------------------------------------------------------------------------------------------------
45211301    1234567890  Mary           Lee            lee35@lapc.edu                        0.00      150.00
43870702    9876543210  John           Smith          jsmith@gmail.com                      0.00      100.00
61522603    1234567890  Sarah          Johnson        sarah.j@college.edu                   0.00      150.00
17738604    9999999999  Mike           Davis          mdavis_1@work.com                     0.00      100.00
72432005    1111111111  Li             Chen           lichen@university.edu                 0.00      150.00

Enter choice: 3

=== Invalid Records ===
123456789 Bob Jones bjones123@company.com - Invalid SSN (must be 10 digits).
1234567890 A B short@test.edu - Invalid first name (min 2 chars). Invalid last name (min 2 chars).

Enter choice: 4 

=== Transaction Log ===
SYSTEM: Bank Account Processing System started
SUCCESS: Account 45211301 created for Mary Lee
SUCCESS: Account 43870702 created for John Smith
INVALID: 123456789 Bob Jones - Invalid SSN (must be 10 digits).
INVALID: 1234567890 A B - Invalid first name (min 2 chars). Invalid last name (min 2 chars).
SUCCESS: Account 61522603 created for Sarah Johnson
SUCCESS: Account 17738604 created for Mike Davis
SUCCESS: Account 72432005 created for Li Chen
SUMMARY: Processed=7 Valid=5 Invalid=2
INFO: Displayed 5 created accounts
INFO: Displayed 2 invalid records


*/
