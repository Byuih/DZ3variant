
#pragma once
#include<iostream>
#include<map>
#include<vector>
#include <string>

using namespace std;

struct User {
    string password;
    string name;
    vector<string> messages;
};

class Chat {
    map<string, User> users;
public:
    void registerUser(string login, string password, string name) {
        User user;
        user.password = password;
        user.name = name;
        users[login] = user;
        cout << "Пользователь успешно зарегистрировался!" << endl;
    }

    void login(string login, string password) {
        if (users.count(login) == 0) {
            cout << "Пользователь не существует!" << endl;
            return;
        }

        User user = users[login];
        if (user.password == password) {
            cout << "Вход в систему  успешный!" << endl;
            startChat(login);
        }
        else {
            cout << "Неверный пароль!" << endl;
        }
    }

    void startChat(string login) {
        while (true) {
            int choice;
            cout << "1. Отправьте сообщение конкретному пользователю" << endl;
            cout << "2. Отправьте сообщение всем пользователям" << endl;
            cout << "3. Посмотреть сообщения" << endl;
            cout << "4. Выход из системы" << endl;

            cout << "Введите свой выбор: ";
            cin >> choice;

            if (choice == 1) {
                string recipient;
                cout << "Введите логин получателя: ";
                cin >> recipient;

                string message;
                cout << "Введите сообщение: ";
                cin.ignore();
                getline(cin, message);

                sendMessage(login, recipient, message);
            }
            else if (choice == 2) {
                string message;
                cout << "Введите сообщение: ";
                cin.ignore();
                getline(cin, message);

                sendMessageToAll(login, message);
            }
            else if (choice == 3) {
                viewMessages(login);
            }
            else if (choice == 4) {
                cout << "Успешно вышел из системы!" << endl;
                break;
            }
            else {
                cout << "Неверный выбор! Пробовать снова." << endl;
            }
        }
    }

    void sendMessage(string sender, string recipient, string message) {
        if (users.count(recipient) == 0) {
            cout << "Получатель не существует!" << endl;
            return;
        }

        users[recipient].messages.push_back(sender + ": " + message);
        cout << "Сообщение отправлено успешно!" << endl;
    }

    void sendMessageToAll(string sender, string message) {
        for (auto& pair : users) {
            User& user = pair.second;
            user.messages.push_back(sender + ": " + message);
        }
        cout << "Сообщение отправлено успешно!" << endl;
    }

    void viewMessages(string login) {
        if (users.count(login) == 0) {
            cout << "Пользователь не существует!" << endl;
            return;
        }

        cout << "Сообщения для: " << login << ":" << endl;
        User& user = users[login];
        for (const string& message : user.messages) {
            cout << message << endl;
        }
    }
};