#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

enum enMainMenuChoice {eClientsList = 1, eAddClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6,eExit = 7};

enum enTransactionsChoice {eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenu = 4};

struct sBankDetails {
    string accNumber;
    string pinCode;
    string holderName;
    string phone;
    double balance;
    bool markForDelete = false;
};

const string CLIENTS_FILE = "Bank.txt";
void mainMenu();
void transactionsMenu(vector<sBankDetails> &vClients);

string convertRecordToLine(sBankDetails client , string delim = "#//#") {
    string line = "";
    line += client.accNumber + delim;
    line += client.pinCode + delim;
    line += client.holderName + delim;
    line += client.phone + delim;
    line += to_string(client.balance);

    return line;
}

bool markClientForDeleteByAccountNumber(string accNumber, vector<sBankDetails> &vClients) {
    for (sBankDetails& c : vClients) {
        if (c.accNumber == accNumber) {
            c.markForDelete = true;
            return true;
        }
    }
    return false;
}


vector<sBankDetails> SaveCleintsDataToFile(string fileName, vector<sBankDetails> &vClients) {
    fstream myFile;
    myFile.open(fileName,ios::out);
    string line;
    if (myFile.is_open()) {
        for (sBankDetails &c : vClients) {
            if (c.markForDelete == false) {
                line = convertRecordToLine(c);
                myFile << line << endl;
            }
        }
        myFile.close();
    }
    return vClients;
}

vector<string> split(string s , string delim) {
    vector<string> words;
    short pos = 0;
    string subWord = "";
    while ((pos = s.find(delim)) != std::string::npos) {
        subWord = s.substr(0,pos);
        if (subWord != "")
            words.push_back(subWord);
        s.erase(0,pos + delim.length());
    }
    if (s != "")
        words.push_back(s);
    return words;
}

sBankDetails convertLineToRecord(string line,string delim = "#//#") {
    sBankDetails client;
    vector<string> vString = split(line, delim);
    client.accNumber = vString[0];
    client.pinCode = vString[1];
    client.holderName = vString[2];
    client.phone = vString[3];
    client.balance = stod(vString[4]);
    return client;
}

vector<sBankDetails> loadDataFromFile(string fileName) {
    vector<sBankDetails> clients;
    sBankDetails client;
    ifstream file;
    string line;
    file.open(fileName);
    if (file.is_open()) {
        while (getline(file,line)) {
            client = convertLineToRecord(line);
            clients.push_back(client);
        }
    }
    else {
        cout << "\nError : File not found!\n";
    }
    return clients;
}

short readChoice() {
    short num;
    do {
        cin >> num;
    }while (num < 1 || num > 7);
    return num;
}

bool findClientByAccountNumber(string accNumber, sBankDetails &client,vector<sBankDetails> &clients) {
    for (sBankDetails &c : clients) {
        if (c.accNumber == accNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

sBankDetails readClientData(vector<sBankDetails> &vClients) {
    sBankDetails client;
    cout << "Enter Account Number :";
    getline(cin,client.accNumber);
    while (findClientByAccountNumber(client.accNumber,client,vClients)) {
        cout << "Account Number is used by another person, Please Choose another account number";
        getline(cin,client.accNumber);
    }
        cout << "Enter Pincode :";
        getline(cin,client.pinCode);;
        cout << "Enter a name : ";
        getline(cin,client.holderName);
        cout << "Enter a Phone Number : ";
        getline(cin,client.phone);
        cout << "Enter a balance : ";
        cin >> client.balance;

    return client;
}


void addDataLineToFile(string fileName,string line) {
    fstream file;
    file.open(fileName,ios::out | ios::app);
    file << line << endl;
    file.close();
}

void addNewClient(vector<sBankDetails> &vClients) {
    sBankDetails client;
    client = readClientData(vClients);
    addDataLineToFile(CLIENTS_FILE,convertRecordToLine(client));
}

void addClients(vector<sBankDetails> &vClients) {
    cin.ignore();
    char c = 'Y';
    do {
        cout << "Adding new client : \n";
        addNewClient(vClients);
        cout << "Do you want to add more clients ?";
        cin >> c;
        cin.ignore();
    }while (toupper(c) == 'Y');
}

void printClientRecord(sBankDetails Client) {
    cout << "| " << setw(15) << left << Client.accNumber;
    cout << "| " << setw(10) << left << Client.pinCode;
    cout << "| " << setw(40) << left << Client.holderName;
    cout << "| " << setw(12) << left << Client.phone;
    cout << "| " << setw(12) << left << Client.balance << endl;
}

void printAllClients(vector<sBankDetails>& vClients) {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sBankDetails &client : vClients) {
        printClientRecord(client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void deleteClientByAccountNumber(string accNumber, vector<sBankDetails> &vClients) {
    char choice;
    sBankDetails client;

    if (findClientByAccountNumber(accNumber, client, vClients)) {
        printClientRecord(client);
        cout << "\nAre you sure you want to delete this account? (y/n) ";
        cin >> choice;

        if (toupper(choice) == 'Y') {
            markClientForDeleteByAccountNumber(accNumber, vClients);
            SaveCleintsDataToFile(CLIENTS_FILE, vClients);
            cout << "\nAccount deleted successfully!\n";
        }
    }
    else {
        cout << "Account Number not found\n";
    }
}

string readAccountNumber() {
    cin.ignore();
    string acc;
    cout << "Enter the account number :";
    getline(cin,acc);
    return acc;
}

sBankDetails changeClientRecord(string accNumber) {
    sBankDetails client;
    client.accNumber = accNumber;
    cin.ignore();
    cout << "Enter Pincode :";
    getline(cin,client.pinCode);;
    cout << "Enter a name : ";
    getline(cin,client.holderName);
    cout << "Enter a Phone Number : ";
    getline(cin,client.phone);
    cout << "Enter a balance : ";
    cin >> client.balance;

    return client;
}

void updateClientByAccountNumber(string accNumber,vector<sBankDetails> &vClients) {
    char c;
    string line;
    sBankDetails client;
    if (findClientByAccountNumber(accNumber,client,vClients)) {
        printClientRecord(client);
        cout << "\nAre you sure you want to update this client ? (y/n)\n";
        cin >> c;
        if (toupper(c) == 'Y') {
            for (sBankDetails &cv : vClients) {
                if (cv.accNumber == accNumber) {
                    cv = changeClientRecord(accNumber);
                    break;
                }
            }
        }
        SaveCleintsDataToFile(CLIENTS_FILE , vClients);
        cout << "\nClient Updated Successfully\n";
    }
    else
        cout << "\nNo account Number found !\n";
}

void depositBalance(string accNumber,double money,vector<sBankDetails> &vClients) {
    cin.ignore();
    cout <<"Are you sure you want to perform this transaction ? (y/n)\n";
    char c;
    cin >> c;
    if (toupper(c) == 'Y')
        for (sBankDetails &client : vClients) {
            if (client.accNumber == accNumber) {
                client.balance += money;
                cout << "New Balance for Account Number [" << accNumber << "] : " << client.balance << "$\n";
            }
        }
    SaveCleintsDataToFile(CLIENTS_FILE,vClients);
}

void depositScreen(vector<sBankDetails> &vClients) {
    double money = 0;
    string accNumber;
    sBankDetails client;
    accNumber = readAccountNumber();

    while (!findClientByAccountNumber(accNumber,client,vClients)) {
        cout << "Account Number [" << accNumber << "] is not found\n";
        cout << "Enter the account number :";
        accNumber = readAccountNumber();
        cin.ignore();
    }
    printClientRecord(client);
    cout << "\nPlease Enter deposit amount :";
    cin >> money;
    while (money < 0) {
        cout <<"You can't deposit money in negative, Please enter a positive value !\n";
        cin >> money;
    }
    depositBalance(accNumber,money,vClients);
}

void withdrawBalance(string accNumber,double money,vector<sBankDetails> &vClients) {
    cin.ignore();
    cout <<"Are you sure you want to perform this transaction ? (y/n)\n";
    char c;
    cin >> c;
    if (toupper(c) == 'Y')
        for (sBankDetails &client : vClients) {
            if (client.accNumber == accNumber) {
                client.balance -= money;
                cout << "New Balance for Account Number [" << accNumber << "] : " << client.balance << "$\n";
            }
        }
    SaveCleintsDataToFile(CLIENTS_FILE,vClients);
}

void withdrawScreen(vector<sBankDetails> &vClients) {
    double money;
    string accNumber;
    sBankDetails client;
    accNumber = readAccountNumber();
    while (!findClientByAccountNumber(accNumber,client,vClients)) {
        cout << "Account Number [" << accNumber << "] is not found\n";
        cout << "Enter the account number :";
        accNumber = readAccountNumber();
    }
    printClientRecord(client);
    cout << "\nPlease Enter withdraw amount :";
    cin >> money;
    while (money < 0) {
        cout <<"You can't withdraw money in negative or amount higher than your balance, Please enter a new value !\n";
        cin >> money;
    }
    withdrawBalance(accNumber,money,vClients);
}

void performMainMenu(enMainMenuChoice choice,vector<sBankDetails> &vClients) {
    sBankDetails client;
        switch (choice) {
            case enMainMenuChoice::eClientsList:
                printAllClients(vClients);
                system("pause");
                break;
            case enMainMenuChoice::eAddClient:
                addClients(vClients);
                system("pause");
                break;
            case enMainMenuChoice::eDeleteClient:
                deleteClientByAccountNumber(readAccountNumber(),vClients);
                system("pause");
                break;
            case enMainMenuChoice::eUpdateClient:
                updateClientByAccountNumber(readAccountNumber(),vClients);
                break;
            case enMainMenuChoice::eFindClient:
                if (findClientByAccountNumber(readAccountNumber(),client,vClients)) {
                    printClientRecord(client);
                }
                else {
                    cout << "Account Number is not found";
                }
                system("pause");
                break;
            case enMainMenuChoice::eTransactions:
                transactionsMenu(vClients);
                system("pause");
            default:
                break;
        }
    }

void mainMenu(vector<sBankDetails> &vClients) {
    short choice;
    do {
        cout << "=============================\n";
        cout << "\tMain Menu Screen\n";
        cout << "=============================\n";
        cout << "[ 1 ] Show Client List\n";
        cout << "[ 2 ] Add New Client\n";
        cout << "[ 3 ] Delete Client\n";
        cout << "[ 4 ] Update Client Info\n";
        cout << "[ 5 ] Find Client\n";
        cout << "[ 6 ] Transactions" << "\n";
        cout << "[ 7 ] Exit" << endl;
        cout << "=============================\n";
        cout << "Choose [1 to 7]: ";

        choice = readChoice();
        performMainMenu(static_cast<enMainMenuChoice>(choice),vClients);
    } while (static_cast<enMainMenuChoice>(choice) != enMainMenuChoice::eExit);
}

void printClientRecordBalanceLine(sBankDetails Client)
{

    cout << "| " << setw(15) << left << Client.accNumber;
    cout << "| " << setw(40) << left << Client.holderName;
    cout << "| " << setw(12) << left << Client.balance;

}

void showTotalBalances()
{

    vector <sBankDetails> vClients = loadDataFromFile(CLIENTS_FILE);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sBankDetails &client : vClients)
        {

            printClientRecordBalanceLine(client);
            TotalBalances += client.balance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances << endl;

}

void performTransactions(enTransactionsChoice choice,vector<sBankDetails> &vClients) {
    switch (choice) {
        case enTransactionsChoice::eDeposit:
            depositScreen(vClients);
            system("pause");
            transactionsMenu(vClients);
            break;
        case enTransactionsChoice::eWithdraw:
            withdrawScreen(vClients);
            system("pause");
            transactionsMenu(vClients);
            break;
        case enTransactionsChoice::eTotalBalances:
            showTotalBalances();
            system("pause");
            transactionsMenu(vClients);
            break;
        case enTransactionsChoice::eMainMenu:
            mainMenu(vClients);
    }
}

void transactionsMenu(vector<sBankDetails> &vClients) {
    cout << "Welcome to Transactions screen, Please select number from below : \n";
    cout << "================================================================\n";
    cout << "[ 1 ] Deposit\n";
    cout << "[ 2 ] Withdraw \n";
    cout << "[ 3 ] Total Balances\n";
    cout << "[ 4 ] Main Menu" << endl;

    performTransactions(static_cast<enTransactionsChoice>(readChoice()),vClients);
}

int main() {
    vector<sBankDetails> vClients = loadDataFromFile(CLIENTS_FILE);
    mainMenu(vClients);
    // system("pause");
    return 0;
}