# Library Management System

This is a simple Library Management System implemented in C++. It allows users to borrow, return, and reserve books. The system supports three types of users: Students, Faculty, and Librarians.

## Features

- **Students** and **Faculty** can:
  - Borrow books
  - Return books
  - View and pay fines
  - Reserve books
  - View all books
  - View their transaction history
  - View their borrowed books
  - Cancel reservations

- **Librarians** can:
  - View all books
  - View all users
  - Add new books
  - Remove books
  - Update book details
  - Add new users
  - Remove users
  - View all transactions

## Prerequisites

- A C++ compiler (e.g., g++, clang++)
- Standard C++ library

## Files

- `main.cpp`: The main source code file.
- `users.txt`: Contains user information.
- `books.txt`: Contains book information.
- `transactions.txt`: Contains transaction history.

## Compilation

To compile the program, use the following command:

```sh
g++ -o LibraryManagementSystem main.cpp
```

This will generate an executable file named `LibraryManagementSystem`.

## Execution

To run the program, use the following command:

```sh
./LibraryManagementSystem
```
### Note
Ensure that the `users.txt` and `books.txt`  and `transactions.txt` files are in the same directory as the executable.

## Usage

1. **Login**: Enter your username and password to log in.
2. **User Menu**: Depending on your role, you will see different options in the menu.
3. **Perform Actions**: Follow the prompts to borrow, return, reserve books, etc.
4. **Logout**: Choose the logout option to log out of the system.
5. **Exit**: Choose the exit option to terminate the program.

## Data Files

- **users.txt**: Stores user information in the format `username,password,name,role,fine`.
- **books.txt**: Stores book information in the format `ISBN,title,author,publisher,year,status`.
- **transactions.txt**: Stores transaction history in the format `username,ISBN,action,date`.

## Example

Here is an example of how the `users.txt` file looks:

```plaintext
student1,pass123,Alice Smith,Student,0
student2,pass123,Bob Johnson,Student,0
faculty1,pass123,Dr. Emily White,Faculty,0
librarian1,admin123,Admin User,Librarian,0
```

And an example of the `books.txt` file:

```plaintext
001,The Catcher in the Rye,J.D. Salinger,Little Brown,1951,Available
002,To Kill a Mockingbird,Harper Lee,J.B. Lippincott,1960,Available
```
