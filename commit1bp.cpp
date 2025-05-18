#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class UserAccount {
    private:
        string username;
        string fullname;
        string passwordHash;
        string phonenumber;
        string WalletID;
        bool isManager;
        bool firstlogin;
    public:
        UserAccount(const string& uname, const string& name, const string& phone, bool manager = false);
        void setPassword(const string& pw);
        void setFirstlogin(bool status);
        string getPasswordHash() const;
        string getUsername() const;
        string getFullname() const;
        string getPhonenumber() const;
        string getWalletID() const;
        bool getisManager() const;
        bool getFirstlogin() const;
        void printInfo() const;
};

UserAccount::UserAccount(const string& uname, const string& name, const string& phone, bool manager) {
    username = uname;
    fullname = name;
    phonenumber = phone;
    isManager = manager;
    firstlogin = true;
    WalletID = "WalletID_" + uname;
}

void UserAccount::setPassword(const string& pw) {
    passwordHash = pw;
}

void UserAccount::setFirstlogin(bool status) {
    firstlogin = status;
}

string UserAccount::getUsername() const { return username; }
string UserAccount::getPasswordHash() const { return passwordHash; }
string UserAccount::getFullname() const { return fullname; }
string UserAccount::getPhonenumber() const { return phonenumber; }
string UserAccount::getWalletID() const { return WalletID; }
bool UserAccount::getisManager() const { return isManager; }
bool UserAccount::getFirstlogin() const { return firstlogin; }

void UserAccount::printInfo() const {
    cout << "Username: " << username << endl;
    cout << "Full Name: " << fullname << endl;
    cout << "Phone Number: " << phonenumber << endl;
    cout << "Wallet ID: " << WalletID << endl;
    cout << "Is Manager: " << (isManager ? "Yes" : "No") << endl;
    cout << "First Login: " << (firstlogin ? "Yes" : "No") << endl;
}

string fakehash(const string& input) {
    string hashed;
    for (char c : input) hashed += (c + 3);
    return hashed;
}

string generateRandompassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password;
    for (int i = 0; i < length; i++) password += chars[rand() % chars.size()];
    return password;
}

bool duplicateuname(const string& user) {
    ifstream inFile("users.txt");
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string uname, dummy;
        getline(ss, uname, ',');
        if (user == uname) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

UserAccount createUserfrominput() {
    string uname, name, phone, password;
    bool isManager;
    while (true) {
        cout << "Username: ";
        getline(cin, uname);
        if (duplicateuname(uname)) {
            cout << "[x] Username existed";
            continue;
        }
        cout << "Fullname: ";
        getline(cin, name);
        cout << "PhoneNumber: ";
        getline(cin, phone);
        cout << "Is Manager ? (1 = Yes, 0 = No) : ";
        cin >> isManager;
        cin.ignore();
        cout << "Input password (empty to create random password): ";
        getline(cin, password);
        if (password.empty()) {
            password = generateRandompassword(8);
            cout << "Your random password: " << password << endl;
        }
        break;
    }
    UserAccount user(uname, name, phone, isManager);
    user.setPassword(fakehash(password));
    return user;
}

void saveUsertofile(const UserAccount& user, const string& filename) {
    ofstream file(filename, ios::app);
    file << user.getUsername() << "," << user.getFullname() << "," << user.getPhonenumber() << ","
         << user.getPasswordHash() << "," << user.getWalletID() << "," << user.getisManager() << "," << user.getFirstlogin() << endl;
    file.close();
}

bool updatePasswordInFile(const string& username, const string& newPassword, const string& filename) {
    ifstream inFile(filename);
    ofstream tempFile("temp.txt");
    string line;
    bool updated = false;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string uname, fullName, phone, pw, wallet;
        bool isManager, firstLogin;
        getline(ss, uname, ',');
        getline(ss, fullName, ',');
        getline(ss, phone, ',');
        getline(ss, pw, ',');
        getline(ss, wallet, ',');
        ss >> isManager;
        ss.ignore();
        ss >> firstLogin;
        if (uname == username) {
            pw = newPassword;
            firstLogin = false;
            updated = true;
        }
        tempFile << uname << ',' << fullName << ',' << phone << ',' << pw << ',' << wallet << ',' << isManager << ',' << firstLogin << '\n';
    }
    inFile.close();
    tempFile.close();
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
    return updated;
}

bool loginAndHandleFirstLogin(const string& username, const string& password) {
    ifstream inFile("users.txt");
    string line;
    string hashedPassword = fakehash(password);
    while (getline(inFile, line)) {
        stringstream ss(line);
        string uname, fullName, phone, pw, wallet;
        bool isManager, firstLogin;
        getline(ss, uname, ',');
        getline(ss, fullName, ',');
        getline(ss, phone, ',');
        getline(ss, pw, ',');
        getline(ss, wallet, ',');
        ss >> isManager;
        ss.ignore();
        ss >> firstLogin;
        if (uname == username && pw == hashedPassword) {
            if (firstLogin) {
                cout << "[!] First login. Change your password please:\nInput new password: ";
                string newPass;
                getline(cin, newPass);
                inFile.close();
                updatePasswordInFile(username, fakehash(newPass), "users.txt");
                cout << "[✓] Password changed.\n";
            } else {
                cout << "[✓] Successful login.\n";
            }
            return true;
        }
    }
    return false;
}
// admins and users panel -minh
void showUserMenu(const string& username) {
    cout << "\n===== USER MENU =====\n";
    cout << "1. Xem thông tin cá nhân\n";
    cout << "2. Đổi mật khẩu\n";
    cout << "3. Thoát\n";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string uname, fullname, phone, pw, wallet;
            bool isManager, firstLogin;
            getline(ss, uname, ',');
            getline(ss, fullname, ',');
            getline(ss, phone, ',');
            getline(ss, pw, ',');
            getline(ss, wallet, ',');
            ss >> isManager;
            ss.ignore();
            ss >> firstLogin;
            if (uname == username) {
                cout << "\nUsername: " << uname
                     << "\nFullname: " << fullname
                     << "\nPhone: " << phone
                     << "\nWallet ID: " << wallet
                     << "\nRole: " << (isManager ? "Admin" : "User") << endl;
                break;
            }
        }
        file.close();
    } else if (choice == 2) {
        cout << "Nhập mật khẩu mới: ";
        string newPass;
        getline(cin, newPass);
        updatePasswordInFile(username, fakehash(newPass), "users.txt");
        cout << "[✓] Mật khẩu đã được thay đổi.\n";
    }
}

void showAdminMenu() {
    cout << "\n===== ADMIN MENU =====\n";
    cout << "1. Xem danh sách người dùng\n";
    cout << "2. Tạo tài khoản mới\n";
    cout << "3. Thoát\n";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
        ifstream file("users.txt");
        string line;
        cout << "\n--- Danh sách người dùng ---\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string uname, fullname, phone, pw, wallet;
            bool isManager, firstLogin;
            getline(ss, uname, ',');
            getline(ss, fullname, ',');
            getline(ss, phone, ',');
            getline(ss, pw, ',');
            getline(ss, wallet, ',');
            ss >> isManager;
            ss.ignore();
            ss >> firstLogin;
            cout << "Username: " << uname << ", Fullname: " << fullname
                 << ", Phone: " << phone << ", Role: " << (isManager ? "Admin" : "User") << endl;
        }
        file.close();
    } else if (choice == 2) {
        UserAccount user = createUserfrominput();
        saveUsertofile(user, "users.txt");
        cout << "[✓] Tạo tài khoản thành công.\n";
    }
}
//                ~~ NO OTP OPTIONS AND FUNCTIONALITY YET, SUBJECT TO CHANGE ~~ -minh
int main() {
    srand(time(0));
    int choice;
    cout << "1. Register\n2. Sign In\nMake choice: ";
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
        UserAccount user = createUserfrominput();
        saveUsertofile(user, "users.txt");
        cout << "[✓] Tạo tài khoản thành công.\n";
        user.printInfo();
    } else if (choice == 2) {
        string uname, password;
        cout << "Input username: ";
        getline(cin, uname);
        cout << "Input password: ";
        getline(cin, password);
        if (!loginAndHandleFirstLogin(uname, password)) {
            cout << "[X] Sai thông tin đăng nhập.\n";
        } else {
            ifstream inFile("users.txt");
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string _uname, fullname, phone, pw, wallet;
                bool isManager, firstLogin;
                getline(ss, _uname, ',');
                getline(ss, fullname, ',');
                getline(ss, phone, ',');
                getline(ss, pw, ',');
                getline(ss, wallet, ',');
                ss >> isManager;
                ss.ignore();
                ss >> firstLogin;
                if (_uname == uname) {
                    if (isManager)
                        showAdminMenu();
                    else
                        showUserMenu(uname);
                    break;
                }
            }
            inFile.close();
        }
    } else {
        cout << "[X] Invalid selection.\n";
    }
    return 0;
}