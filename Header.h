
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
        cout << "������������ ������� �����������������!" << endl;
    }

    void login(string login, string password) {
        if (users.count(login) == 0) {
            cout << "������������ �� ����������!" << endl;
            return;
        }

        User user = users[login];
        if (user.password == password) {
            cout << "���� � �������  ��������!" << endl;
            startChat(login);
        }
        else {
            cout << "�������� ������!" << endl;
        }
    }

    void startChat(string login) {
        while (true) {
            int choice;
            cout << "1. ��������� ��������� ����������� ������������" << endl;
            cout << "2. ��������� ��������� ���� �������������" << endl;
            cout << "3. ���������� ���������" << endl;
            cout << "4. ����� �� �������" << endl;

            cout << "������� ���� �����: ";
            cin >> choice;

            if (choice == 1) {
                string recipient;
                cout << "������� ����� ����������: ";
                cin >> recipient;

                string message;
                cout << "������� ���������: ";
                cin.ignore();
                getline(cin, message);

                sendMessage(login, recipient, message);
            }
            else if (choice == 2) {
                string message;
                cout << "������� ���������: ";
                cin.ignore();
                getline(cin, message);

                sendMessageToAll(login, message);
            }
            else if (choice == 3) {
                viewMessages(login);
            }
            else if (choice == 4) {
                cout << "������� ����� �� �������!" << endl;
                break;
            }
            else {
                cout << "�������� �����! ��������� �����." << endl;
            }
        }
    }

    void sendMessage(string sender, string recipient, string message) {
        if (users.count(recipient) == 0) {
            cout << "���������� �� ����������!" << endl;
            return;
        }

        users[recipient].messages.push_back(sender + ": " + message);
        cout << "��������� ���������� �������!" << endl;
    }

    void sendMessageToAll(string sender, string message) {
        for (auto& pair : users) {
            User& user = pair.second;
            user.messages.push_back(sender + ": " + message);
        }
        cout << "��������� ���������� �������!" << endl;
    }

    void viewMessages(string login) {
        if (users.count(login) == 0) {
            cout << "������������ �� ����������!" << endl;
            return;
        }

        cout << "��������� ���: " << login << ":" << endl;
        User& user = users[login];
        for (const string& message : user.messages) {
            cout << message << endl;
        }
    }
};