This is a console application written in C++. Its main function is to manage your personal expenditures and keep track of your income and expenses. It also has some other useful features.
## Features
- [x] Allow multiple users
- [x] Add new income/expense
- [x] Edit income/expense
- [x] Delete income/expense
- [x] View income/expense
- [x] View income/expense by date
- [x] View income/expense by category
- [x] Generate reports based on income/expense
- [x] Save generated reports to file
- [x] Delete account
- [x] Change password

## How to install

A pre built executable file is provided, which is a windows executable file. If you want to compile the application yourself, follow the steps below.

### 1. Clone the repository
```
git clone
```
### 2. Compile the application
```
g++ -o main main.cpp
```
### 3. Run the application
```
./main
```
 
Or,If you have GNU GCC compiler installed through minGW, you can use the Makefile to compile and run the application. To do so, navigate to the directory where the Makefile is located and run the following command:
```
mingw32-make
```
## Before you start, make sure to create two folders named "reports" and "users" in the same directory as the application. The reports will be saved in the "reports" folder and the user data will be saved in the "users" folder.

## How to use

### 1. Main menu
When you run the application, you will be asked to login. If you don't have an account, you can create one by choosing option 1. If you already have an account, you can login by choosing option 2. If you want help, you can choose option 4.

### 2. User menu
After you login, you will be presented with the user menu. Here you can choose what you want to do. You can add, edit, delete or view income/expense. You can also generate reports based on income/expense. If you want to change your password, you can choose option 6.If you want to logout, you can choose option 9. If you want to delete your account, you can choose option 0. 

