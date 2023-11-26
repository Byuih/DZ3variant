#include <iostream>
#include <vector>
#include <string>
#include "windows.h"
#include "iostream"
#include "Header.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")


int main()
{
	setlocale(LC_ALL, "ru");
	cout << "----------- Клиент -----------" << endl;
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Ошибка инициализации сокета!" << endl;
	}
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "Не удалось создать сокет!" << endl;
	}
	struct sockaddr_in SeverAddress;		// Адрес сервера - это целевой адрес для подключения
	memset(&SeverAddress, 0, sizeof(sockaddr_in));
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1 представляет локальный IP-адрес
	SeverAddress.sin_port = htons(60000);// Устанавливаем номер порта

	// Запускаем соединение
	if (connect(clientSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "Клиент: не удалось подключиться к серверу!" << endl;
		return 0;
	}
	else
		cout << "Клиент: успешно подключился к серверу!" << endl;
	//tyt
	setlocale(LC_ALL, "ru");
	Chat chat;
	int choice;
	string login, password, name;

	while (true) {
		cout << "1. Зарегистрируйте нового пользователя:" << endl;
		cout << "2. Логин:" << endl;
		cout << "3. Выход." << endl;

		cout << "Введите свой выбор: ";
		cin >> choice;

		if (choice == 1) {
			cout << "Введите логин: ";
			cin >> login;

			cout << "Введите пароль: ";
			cin >> password;

			cout << "Введите имя: ";
			cin.ignore();
			getline(cin, name);

			chat.registerUser(login, password, name);
		}
		else if (choice == 2) {
			cout << "Введите логин: ";
			cin >> login;

			cout << "Введите пароль: ";
			cin >> password;

			chat.login(login, password);
		}
		else if (choice == 3) {
			cout << "Выхожу..." << endl;
			break;
		}
		else {
			cout << "Неверный выбор! Пробовать снова." << endl;
		}
	}

	//tyt
	// Клиент отправляет данные на сервер
	char sendbuf[2048] = "from client:hello server.";
	if (send(clientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
	{
		cout << "Не удалось отправить сообщение!";
	}

	// Клиент принимает данные с сервера
	char  recvbuf[2048] = {};
	if (recv(clientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
	{
		cout << "Не удалось принять данные" << endl;
	}
	else
		cout << "Данные, полученные клиентом : " << recvbuf << endl;

	// Закрываем сокет
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}

	// завершить
	WSACleanup();
	cout << "Клиент уходит!" << endl;
	return 0;
}

struct Message {
	std::string username;
	std::string content;
	bool isPrivate;
};

class Chat {
private:
	std::vector<Message> messages;
	std::vector<std::string> users;
	std::vector<std::string> bannedUsers;
public:
	void sendMessage(const std::string& username, const std::string& content, bool isPrivate = false) {
		if (isUserBanned(username)) {
			std::cout << "You are banned and cannot send messages." << std::endl;
			return;
		}

		Message message;
		message.username = username;
		message.content = content;
		message.isPrivate = isPrivate;

		messages.push_back(message);
	}

	void viewAllMessages() const {
		for (const auto& message : messages) {
			std::cout << "[" << message.username << "]: " << message.content << std::endl;
		}
	}

	void viewPrivateMessages(const std::string& username) const {
		for (const auto& message : messages) {
			if (message.isPrivate && (message.username == username || message.content.find("@" + username) != std::string::npos)) {
				std::cout << "[" << message.username << " to " << username << "]: " << message.content << std::endl;
			}
		}
	}

	void viewUsers() const {
		std::cout << "Users: ";
		for (const auto& user : users) {
			std::cout << user << " ";
		}
		std::cout << std::endl;
	}

	void banUser(const std::string& username) {
		bannedUsers.push_back(username);
		std::cout << username << " has been banned." << std::endl;
	}

	void disconnectUser(const std::string& username) {
		auto it = std::find(users.begin(), users.end(), username);
		if (it != users.end()) {
			users.erase(it);
			std::cout << username << " has been disconnected." << std::endl;
		}
		else {
			std::cout << username << " is not online." << std::endl;
		}
	}

	void addUser(const std::string& username) {
		users.push_back(username);
		std::cout << username << " has joined the chat." << std::endl;
	}

	bool isUserBanned(const std::string& username) const {
		return std::find(bannedUsers.begin(), bannedUsers.end(), username) != bannedUsers.end();
	}
};

int main() {
	Chat chat;
	chat.addUser("User1");
	chat.addUser("User2");
	chat.addUser("User3");

	chat.sendMessage("User1", "Hello everyone!");

	chat.viewAllMessages();
	chat.viewPrivateMessages("User2");
	chat.viewPrivateMessages("User3");

	chat.banUser("User2");
	chat.disconnectUser("User1");

	chat.viewUsers();

	return 0;
}