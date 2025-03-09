#include <bits/stdc++.h>
using namespace std;

class Account {
private:
    vector<string> currentBorrowed;
    map<string, time_t> dueDates;
    vector<string> borrowingHistory;
    double fine;
public:
    Account() : fine(0) {}
    
    void addFine(double amount) {
        fine += amount;
    }
    void payFine() {
        fine = 0;
    }
    double getFine() const {
        return fine;
    }
    void addBorrowed(const string& isbn, time_t dueDate) {
        currentBorrowed.push_back(isbn);
        dueDates[isbn] = dueDate;
    }
    void removeBorrowed(const string& isbn) {
        auto it = find(currentBorrowed.begin(), currentBorrowed.end(), isbn);
        if (it != currentBorrowed.end()) {
            currentBorrowed.erase(it);
        }
        dueDates.erase(isbn);
    }
    const vector<string>& getCurrentBorrowed() const {
        return currentBorrowed;
    }
    time_t getDueDate(const string& isbn) const {
        auto it = dueDates.find(isbn);
        return it != dueDates.end() ? it->second : 0;
    }
    void addToHistory(const string& isbn) {
        borrowingHistory.push_back(isbn);
    }
};

class User {
protected:
    string username;
    string password;
    string name;
    string role;
    Account account;
public:
    User(string u, string p, string n, string r)  : username(u), password(p), name(n), role(r) {}
    virtual ~User() {}
    
    string getUsername() const {
        return username;
    }
    string getPassword() const {
        return password;
    }
    string getName() const {
        return name;
    }
    string getRole() const {
        return role;
    }
    Account& getAccount() {
        return account;
    }
    void setPassword(string p) {
        password = p;
    }
    void setName(string n) {
        name = n;
    }

    virtual bool canBorrowAnother() const = 0;
    virtual int getMaxBooks() const = 0;
    virtual int getMaxBorrowDays() const = 0;
    virtual double calculateFine(int days) const = 0;
    virtual bool borrowBook(string isbn) = 0;
    virtual bool returnBook(string isbn) = 0;
};

class Student : public User {
public:
    Student(string u, string p, string n) : User(u, p, n, "Student") {}
    
    bool canBorrowAnother() const {
        return account.getCurrentBorrowed().size() < getMaxBooks();
    }
    int getMaxBooks() const {
        return 3;
    }
    int getMaxBorrowDays() const {
        return 15;
    }
    double calculateFine(int days) const {
        return days * 10;
    }
    bool borrowBook(string isbn) {
        if (canBorrowAnother()) {
            time_t due = time(nullptr) + getMaxBorrowDays() * 86400;
            account.addBorrowed(isbn, due);
            return true;
        }
        return false;
    }
    bool returnBook(string isbn) {
        auto& borrowed = account.getCurrentBorrowed();
        auto it = find(borrowed.begin(), borrowed.end(), isbn);
        if (it != borrowed.end()) {
            account.removeBorrowed(isbn);
            account.addToHistory(isbn);
            return true;
        }
        return false;
    }
};

class Faculty : public User {
public:
    Faculty(string u, string p, string n) : User(u, p, n, "Faculty") {}
    
    bool canBorrowAnother() const {
        return account.getCurrentBorrowed().size() < getMaxBooks();
    }
    int getMaxBooks() const {
        return 5;
    }
    int getMaxBorrowDays() const {
        return 30;
    }
    double calculateFine(int days) const {
        return 0;
    }
    bool borrowBook(string isbn) {
        if (canBorrowAnother()) {
            time_t due = time(nullptr) + getMaxBorrowDays() * 86400;
            account.addBorrowed(isbn, due);
            return true;
        }
        return false;
    }
    bool returnBook(string isbn) {
        auto& borrowed = account.getCurrentBorrowed();
        auto it = find(borrowed.begin(), borrowed.end(), isbn);
        if (it != borrowed.end()) {
            account.removeBorrowed(isbn);
            account.addToHistory(isbn);
            return true;
        }
        return false;
    }
};

class Librarian : public User {
public:
    Librarian(string u, string p, string n) : User(u, p, n, "Librarian") {}
    
    bool canBorrowAnother() const {
        return false;
    }
    int getMaxBooks() const {
        return 0;
    }
    int getMaxBorrowDays() const {
        return 0;
    }
    double calculateFine(int days) const {
        return 0;
    }
    bool borrowBook(string isbn) {
        return false;
    }
    bool returnBook(string isbn) {
        return false;
    }
};

class Book {
private:
    string ISBN;
    string title;
    string author;
    string publisher;
    int year;
    string status;
public:
    Book(string i, string t, string a, string p, int y, string s) : ISBN(i), title(t), author(a), publisher(p), year(y), status(s) {}
    
    string getISBN() const {
        return ISBN;
    }
    string getTitle() const {
        return title;
    }
    string getAuthor() const {
        return author;
    }
    string getPublisher() const {
        return publisher;
    }
    int getYear() const {
        return year;
    }
    string getStatus() const {
        return status;
    }
    void setStatus(string s) {
        status = s;
    }
    void setTitle(string t) {
        title = t;
    }
    void setAuthor(string a) {
        author = a;
    }
    void setPublisher(string p) {
        publisher = p;
    }
    void setYear(int y) {
        year = y;
    }
};

class Library {
private:
    vector<User*> users;
    vector<Book> books;
    map<string, string> reservations;

public:
    ~Library() {
        for (auto user : users) {
            delete user;
        }
    }

    string currentTime() {
        time_t now = time(0);
        return ctime(&now);
    }

    void loadUsers(const string& filename) {
        ifstream file(filename);
        if (!file) {
            return;
        }
        string line;
        while (getline(file, line)) {
            vector<string> parts;
            size_t pos = 0;
            while ((pos = line.find(',')) != string::npos) {
                parts.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            parts.push_back(line);
            if (parts.size() != 5) {
                continue;
            }
            User* user = nullptr;
            if (parts[3] == "Student")
                user = new Student(parts[0], parts[1], parts[2]);
            else if (parts[3] == "Faculty")
                user = new Faculty(parts[0], parts[1], parts[2]);
            else if (parts[3] == "Librarian")
                user = new Librarian(parts[0], parts[1], parts[2]);
            else {
                continue;
            }
            user->getAccount().addFine(stod(parts[4]));
            users.push_back(user);
        }
        file.close();
    }
    void loadBooks(const string& filename) {
        ifstream file(filename);
        if (!file) {
            return;
        }
        string line;
        while (getline(file, line)) {
            vector<string> parts;
            size_t pos = 0;
            while ((pos = line.find(',')) != string::npos) {
                parts.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            parts.push_back(line);
            if (parts.size() != 6) {
                continue;
            }
            books.emplace_back(parts[0], parts[1], parts[2], parts[3], stoi(parts[4]), parts[5]);
        }
        file.close();
    }
    void loadTransactions(const string& filename) {
        ifstream file(filename);
        if (!file) {
            return;
        }
        string line;
        vector<vector<string>> transactions;
        while (getline(file, line)) {
            vector<string> parts;
            size_t pos = 0;
            while ((pos = line.find(',')) != string::npos) {
                parts.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            parts.push_back(line);
            if (parts.size() != 4) {
                continue;
            }
            transactions.push_back(parts);
        }
        reverse(transactions.begin(), transactions.end());
        for (const auto& parts : transactions) {
            string username = parts[0];
            string isbn = parts[1];
            string action = parts[2];
            time_t timestamp;
            struct tm tm = {};
            istringstream ss(parts[3]);
            ss >> get_time(&tm, "%a %b %d %H:%M:%S %Y");
            if (ss.fail()) {
                cerr << "Failed to parse timestamp: " << parts[3] << endl;
                continue;
            }
            timestamp = mktime(&tm);

            User* user = findUser(username);
            Book* book = findBook(isbn);
            if (user && book) {
                if (action == "borrow") {
                    reservations.erase(isbn);
                    user->getAccount().addBorrowed(isbn, timestamp);
                    book->setStatus("Borrowed");
                } else if (action == "return") {
                    user->getAccount().removeBorrowed(isbn);
                    user->getAccount().addToHistory(isbn);
                    book->setStatus("Available");
                } else if (action == "reserve") {
                    reservations[isbn] = username;
                    book->setStatus("Reserved");
                } else if (action == "cancel_reservation") {
                    reservations.erase(isbn);
                    book->setStatus("Borrowed");
                }
            }
        }
        file.close();
    }

    void saveUsers(const string& filename) {
        ofstream file(filename);
        if (!file) {
            return;
        }
        for (auto user : users) {
            file << user->getUsername() << ","
                 << user->getPassword() << ","
                 << user->getName() << ","
                 << user->getRole() << ","
                 << user->getAccount().getFine() << "\n";
        }
        file.close();
    }
    void saveBooks(const string& filename) {
        ofstream file(filename);
        if (!file) {
            return;
        }
        for (auto& book : books) {
            file << book.getISBN() << ","
                 << book.getTitle() << ","
                 << book.getAuthor() << ","
                 << book.getPublisher() << ","
                 << book.getYear() << ","
                 << book.getStatus() << "\n";
        }
        file.close();
    }
    void saveTransaction(const string& user, const string& ISBN, const string& action, const string& time) {
        ofstream file("transactions.txt", ios::app);
        if (file) {
            file << user << "," << ISBN << "," << action << "," << time;
        }
        file.close();
    }

    User* login(const string& username, const string& password) {
        for (auto user : users) {
            if (user->getUsername() == username && user->getPassword() == password) {
                return user;
            }
        }
        return nullptr;
    }

    Book* findBook(const string& ISBN) {
        for (auto& book : books) {
            if (book.getISBN() == ISBN) {
                return &book;
            }
        }
        return nullptr;
    }
    bool borrowBook(User* user, const string& ISBN) {
        if (ISBN == "c") {
            cout << "Operation cancelled.\n";
            return false;
        }
        Book* book = findBook(ISBN);
        if (!book) {
            cout << "Book not found.\n";
            return false;
        }
        if (book->getStatus() != "Available") {
            cout << "Book not available.\n";
            return false;
        }
        if (user->getAccount().getFine() > 0) {
            cout << "Clear fines first.\n";
            return false;
        }
        if (!user->canBorrowAnother()) {
            cout << "Borrow & Reserve limit reached.\n";
            return false;
        }
        if (user->borrowBook(ISBN)) {
            time_t now = time(nullptr);
            time_t due = now + user->getMaxBorrowDays() * 86400;
            user->getAccount().addBorrowed(ISBN, due);
            book->setStatus("Borrowed");
            saveTransaction(user->getUsername(), ISBN, "borrow", currentTime());
            saveUsers("users.txt");
            saveBooks("books.txt");
            cout << "Book borrowed.\n";
            return true;
        }
        return false;
    }
    bool returnBook(User* user, const string& ISBN) {
        if (ISBN == "c") {
            cout << "Operation cancelled.\n";
            return false;
        }
        Book* book = findBook(ISBN);
        if (!book) {
            cout << "Book not found.\n";
            return false;
        }
        if (book->getStatus() != "Borrowed") {
            cout << "Book not borrowed.\n";
            return false;
        }
        auto& borrowed = user->getAccount().getCurrentBorrowed();
        if (find(borrowed.begin(), borrowed.end(), ISBN) == borrowed.end()) {
            cout << "You didn't borrow this book.\n";
            return false;
        }
        time_t dueDate = user->getAccount().getDueDate(ISBN);
        time_t returnTime = time(nullptr);
        int daysOverdue = max(0, (int)(returnTime - dueDate) / 86400);
        if (daysOverdue > 0) {
            double fine = user->calculateFine(daysOverdue);
            user->getAccount().addFine(fine);
            cout << "Overdue by " << daysOverdue << " days. Fine: " << fine << "\n";
        }
        user->getAccount().removeBorrowed(ISBN);
        user->getAccount().addToHistory(ISBN);
        saveTransaction(user->getUsername(), ISBN, "return", currentTime());
        if (reservations.find(ISBN) != reservations.end()) {
            string resUser = reservations[ISBN];
            User* reservedUser = findUser(resUser);
            if (reservedUser && reservedUser->canBorrowAnother()) {
                reservedUser->borrowBook(ISBN);
                book->setStatus("Borrowed");
                saveTransaction(reservedUser->getUsername(), ISBN, "borrow", currentTime());
            } else {
                book->setStatus("Available");
            }
            reservations.erase(ISBN);
        } else {
            book->setStatus("Available");
        }
        saveUsers("users.txt");
        saveBooks("books.txt");
        cout << "Book returned.\n";
        return true;
    }
    void reserveBook(User* user, const string& ISBN) {
        if (ISBN == "c") {
            cout << "Operation cancelled.\n";
            return;
        }
        Book* book = findBook(ISBN);
        if (!book) {
            cout << "Book not found.\n";
            return;
        }
        if (book->getStatus() == "Available") {
            cout << "Book not available for borrowing, not reservation.\n";
            return;
        }
        if (book->getStatus() == "Reserved") {
            cout << "Already reserved.\n";
            return;
        }
        if (user->getAccount().getFine() > 0) {
            cout << "Clear fines first.\n";
            return;
        }
        
        if (!user->canBorrowAnother()) {
            cout << "Borrow & reserve limit reached.\n";
            return;
        }
        reservations[ISBN] = user->getUsername();
        book->setStatus("Reserved");
        saveTransaction(user->getUsername(), ISBN, "reserve", currentTime());
        saveUsers("users.txt");
        saveBooks("books.txt");
        cout << "Reserved successfully.\n";
    }
    bool addBook(const Book& book) {
        books.push_back(book);
        saveBooks("books.txt");
        return true;
    }
    bool removeBook(const string& ISBN) {
        if (ISBN == "c") {
            cout << "Operation cancelled.\n";
            return false;
        }
        auto it = find_if(books.begin(), books.end(), [&](const Book& b) {
            return b.getISBN() == ISBN;
        });
        if (it != books.end()) {
            books.erase(it);
            saveBooks("books.txt");
            return true;
        }
        return false;
    }

    User* findUser(const string& username) {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr;
    }
    bool addUser(User* user) {
        if (findUser(user->getUsername())) {
            return false;
        }
        users.push_back(user);
        saveUsers("users.txt");
        return true;
    }
    void removeUser(const string& username, const string& my) {
        if (username == "c") {
            cout << "Operation cancelled.\n";
            return;
        }
        auto it = find_if(users.begin(), users.end(), [&](User* u) {
            return u->getUsername() == username;
        });
        if (it != users.end()) {
            if ((*it)->getUsername() == my) {
                cout << "Cannot remove yourself.\n";
                return;
            }
            delete *it;
            users.erase(it);
            saveUsers("users.txt");
            cout << "User removed.\n";
            return;
        }
        cout << "User not found.\n";
        return;
    }
    void cancelReservation(User* user, const string& ISBN) {
        if (ISBN == "c") {
            cout << "Operation cancelled.\n";
            return;
        }
        auto it = reservations.find(ISBN);
        if (it != reservations.end() && it->second == user->getUsername()) {
            reservations.erase(it);
            Book* book = findBook(ISBN);
            if (book) {
                book->setStatus("Borrowed");
            }
            saveTransaction(user->getUsername(), ISBN, "cancel_reservation", currentTime());
            cout << "Reservation cancelled.\n";
        } else {
            cout << "No reservation found for this book.\n";
        }
    }

    void displayAllBooks() {
        cout << "\n--- Books ---\n";
        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }
        for (const auto& book : books) {
            cout << "ISBN: " << book.getISBN() 
                 << "\nTitle: " << book.getTitle()
                 << "\nAuthor: " << book.getAuthor()
                 << "\nPublisher: " << book.getPublisher()
                 << "\nYear: " << book.getYear()
                 << "\nStatus: " << book.getStatus();
            if (book.getStatus() == "Borrowed") {
                time_t dueDate = 0;
                for (auto user : users) {
                    const auto& borrowed = user->getAccount().getCurrentBorrowed();
                    if (find(borrowed.begin(), borrowed.end(), book.getISBN()) != borrowed.end()) {
                        dueDate = user->getAccount().getDueDate(book.getISBN());
                        break;
                    }
                }
                if (dueDate != 0) {
                    cout << "\nDue Date: " << ctime(&dueDate);
                }
            } else if (book.getStatus() == "Reserved") {
                time_t reservationDate = 0;
                for (auto user : users) {
                    const auto& borrowed = user->getAccount().getCurrentBorrowed();
                    if (find(borrowed.begin(), borrowed.end(), book.getISBN()) != borrowed.end()) {
                        reservationDate = user->getAccount().getDueDate(book.getISBN());
                        break;
                    }
                }
                if (reservationDate != 0) {
                    User* reservedUser = findUser(reservations[book.getISBN()]);
                    if (reservedUser) {
                        reservationDate += reservedUser->getMaxBorrowDays() * 86400;
                        cout << "\nReserved Till: " << ctime(&reservationDate);
                    }
                }
            }
            cout << "\n\n";
        }
    }
    void displayAllUsers() {
        cout << "\n--- Users ---\n";
        for (const auto& user : users) {
            cout << "Username: " << user->getUsername()
                 << "\nName: " << user->getName()
                 << "\nRole: " << user->getRole()
                 << "\nFine: " << user->getAccount().getFine() << "\n\n";
        }
    }

    void displayTransactions() {
        ifstream file("transactions.txt");
        string line;
        bool hasTransactions = false;
        cout << "\n--- Transaction History ---\n";
        while (getline(file, line)) {
            hasTransactions = true;
            vector<string> parts;
            string token;
            istringstream tokenStream(line);
            while (getline(tokenStream, token, ',')) {
                parts.push_back(token);
            }
            if (parts.size() >= 4) {
                cout << "User: " << parts[0] 
                     << " | Book: " << parts[1] 
                     << " | Action: " << parts[2]
                     << " | Date: " << parts[3] << "\n";
            }
        }
        if (!hasTransactions) {
            cout << "No transactions found.\n";
        }
        file.close();
    }
    void displayUserTransactions(const string& username) {
        ifstream file("transactions.txt");
        string line;
        bool hasTransactions = false;
        cout << "\n--- Transaction History ---\n";
        while (getline(file, line)) {
            hasTransactions = true;
            vector<string> parts;
            string token;
            istringstream tokenStream(line);
            while (getline(tokenStream, token, ',')) {
                parts.push_back(token);
            }
            if (parts.size() >= 4 && parts[0] == username) {
                cout << "Book: " << parts[1] 
                     << " | Action: " << parts[2]
                     << " | Date: " << parts[3] << "\n";
            }
        }
        if (!hasTransactions) {
            cout << "No transactions found.\n";
        }
        file.close();
    }
    void displayBorrowedBooks(User* user) {
        const auto& borrowed = user->getAccount().getCurrentBorrowed();
        if (borrowed.empty()) {
            cout << "No borrowed books.\n";
            return;
        }
        for (const auto& isbn : borrowed) {
            Book* book = findBook(isbn);
            if (book) {
                cout << "ISBN: " << book->getISBN() 
                     << "\nTitle: " << book->getTitle()
                     << "\nAuthor: " << book->getAuthor()
                     << "\nPublisher: " << book->getPublisher()
                     << "\nYear: " << book->getYear()
                     << "\nStatus: " << book->getStatus();
                if (book->getStatus() == "Borrowed") {
                    time_t dueDate = user->getAccount().getDueDate(isbn);
                    cout << "\nDue Date: " << ctime(&dueDate);
                }
                cout << "\n\n";
            }
        }
    }

    int userMenu(User* user) {
        int choice;
        do {
            cout << "\n===== Library Management System =====\n";
            cout << "\n1. Borrow Book\n2. Return Book\n3. View/Pay Fine\n4. Reserve Book\n5. View All Books\n6. View My Transactions\n7. View Borrowed Books\n8. Cancel Reservation\n9. Logout\n0. Exit\nChoice: ";
            cin >> choice;
            string ISBN;
            switch (choice) {
                case 1:
                    cout << "ISBN (enter 'c' to cancel): ";
                    cin >> ISBN;
                    borrowBook(user, ISBN);
                    break;
                case 2:
                    cout << "ISBN (enter 'c' to cancel): ";
                    cin >> ISBN;
                    returnBook(user, ISBN);
                    break;
                case 3:
                    cout << "Fine: " << user->getAccount().getFine() << "\n";
                    cout << "Pay fine? (y/n): ";
                    char ch;
                    cin >> ch;
                    if (ch == 'y') {
                        user->getAccount().payFine();
                        cout << "Fines cleared.\n";
                    }
                    break;
                case 4:
                    cout << "ISBN (enter 'c' to cancel): ";
                    cin >> ISBN;
                    reserveBook(user, ISBN);
                    break;
                case 5:
                    displayAllBooks();
                    break;
                case 6:
                    displayUserTransactions(user->getUsername());
                    break;
                case 7:
                    displayBorrowedBooks(user);
                    break;
                case 8:
                    cout << "ISBN (enter 'c' to cancel): ";
                    cin >> ISBN;
                    cancelReservation(user, ISBN);
                    break;
                case 9:
                    cout << "Logging out...\n";
                    return 1;
                case 0:
                    cout << "Exiting...\n";
                    return 0;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
        } while (choice != 0);
        return 0;
    }
};

string getHiddenInput() {
    string password;
    char ch;
    while ((ch = _getwch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

int main() {
    Library lib;
    lib.loadUsers("users.txt");
    lib.loadBooks("books.txt");
    lib.loadTransactions("transactions.txt");

    login:
    cout << "\n===== Library Management System =====\n";
    int choice;
    cout << "\n1. Login\n0. Exit\nChoice: ";
    cin >> choice;
    if (choice == 0) {
        cout << "Exiting...\n";
        return 0;
    } else if (choice != 1) {
        cout << "Invalid choice.\n";
        goto login;
    }
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    password = getHiddenInput();

    User* user = lib.login(username, password);
    if (!user) {
        cout << "Login failed.\n";
        goto login;
    }

    string role = user->getRole();
    if (role == "Student" || role == "Faculty") {
        if (lib.userMenu(user)) {
            goto login;
        }
    } else if (role == "Librarian") {
        int choice;
        do {
            cout << "\n===== Library Management System =====\n";
            cout << "\nLibrarian Menu:\n1. View Books\n2. View Users\n3. Add Book\n4. Remove Book\n5. Update Book\n6. Add User\n7. Remove User\n8. View all transactions\n9. Logout\n0. Exit\nChoice: ";
            cin >> choice;
            string ISBN, title, author, publisher, status, uname, pwd, pwdConfirm, name, role;
            int year;
            Book* book;
            User* existingUser;
            switch (choice) {
                case 1:
                    lib.displayAllBooks();
                    break;
                case 2:
                    lib.displayAllUsers();
                    break;
                case 3:
                    cout << "Enter ISBN (enter 'c' to cancel): ";
                    cin >> ISBN;
                    if (ISBN == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    cin.ignore();
                    cout << "Enter title: ";
                    getline(cin, title);
                    cout << "Enter author: ";
                    getline(cin, author);
                    cout << "Enter publisher: ";
                    getline(cin, publisher);
                    cout << "Enter year: ";
                    cin >> year;
                    lib.addBook(Book(ISBN, title, author, publisher, year, "Available"));
                    break;
                case 4:
                    cout << "Enter ISBN to remove (enter 'c' to cancel): ";
                    cin >> ISBN;
                    if (lib.removeBook(ISBN)) {
                        cout << "Book removed.\n";
                    } else {
                        cout << "Book not found.\n";
                    }
                    break;
                case 5:
                    cout << "Enter ISBN to update (enter 'c' to cancel): ";
                    cin >> ISBN;
                    if (ISBN == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    book = lib.findBook(ISBN);
                    if (book) {
                        cin.ignore();
                        cout << "New title (enter to skip): ";
                        getline(cin, title);
                        if (!title.empty()) {
                            book->setTitle(title);
                        }
                        cout << "New author (enter to skip): ";
                        getline(cin, author);
                        if (!author.empty()) {
                            book->setAuthor(author);
                        }
                        cout << "New publisher (enter to skip): ";
                        getline(cin, publisher);
                        if (!publisher.empty()) {
                            book->setPublisher(publisher);
                        }
                        cout << "New year (enter 0 to skip): ";
                        cin >> year;
                        if (year != 0) {
                            book->setYear(year);
                        }
                        cout << "Book updated.\n";
                    } else {
                        cout << "Book not found.\n";
                    }
                    break;
                case 6: {
                    cout << "Username (enter 'c' to cancel): ";
                    cin >> uname;
                    if (uname == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    cout << "Password: ";
                    pwd = getHiddenInput();
                    if (pwd == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    cout << "Re-enter Password: ";
                    pwdConfirm = getHiddenInput();
                    if (pwdConfirm != pwd) {
                        cout << "Passwords do not match.\n";
                        break;
                    }
                    cin.ignore();
                    cout << "Name: ";
                    getline(cin, name);
                    if (name == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    cout << "Role (Student/Faculty/Librarian): ";
                    cin >> role;
                    if (role == "c") {
                        cout << "Operation cancelled.\n";
                        break;
                    }
                    User* newUser = nullptr;
                    if (role == "Student") {
                        newUser = new Student(uname, pwd, name);
                    } else if (role == "Faculty") {
                        newUser = new Faculty(uname, pwd, name);
                    } else if (role == "Librarian") {
                        newUser = new Librarian(uname, pwd, name);
                    } else {
                        cout << "Invalid role.\n";
                        break;
                    }
                    if (lib.addUser(newUser)) {
                        cout << "User added.\n";
                    } else {
                        cout << "Username already exists.\n";
                    }
                    break;
                }
                case 7:
                    cout << "Enter username to remove (enter 'c' to cancel): ";
                    cin >> uname;
                    lib.removeUser(uname, user->getUsername());
                    break;
                case 8:
                    lib.displayTransactions();
                    break;
                case 9:
                    cout << "Logging out...\n";
                    goto login;
                case 0:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
        } while (choice != 0);
    }
    return 0;
}