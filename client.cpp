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
	cout << "----------- ������ -----------" << endl;
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "������ ������������� ������!" << endl;
	}
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�� ������� ������� �����!" << endl;
	}
	struct sockaddr_in SeverAddress;		// ����� ������� - ��� ������� ����� ��� �����������
	memset(&SeverAddress, 0, sizeof(sockaddr_in));
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1 ������������ ��������� IP-�����
	SeverAddress.sin_port = htons(60000);// ������������� ����� �����

	// ��������� ����������
	if (connect(clientSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "������: �� ������� ������������ � �������!" << endl;
		return 0;
	}
	else
		cout << "������: ������� ����������� � �������!" << endl;
	//tyt
	setlocale(LC_ALL, "ru");
	Chat chat;
	int choice;
	string login, password, name;

	while (true) {
		cout << "1. ��������������� ������ ������������:" << endl;
		cout << "2. �����:" << endl;
		cout << "3. �����." << endl;

		cout << "������� ���� �����: ";
		cin >> choice;

		if (choice == 1) {
			cout << "������� �����: ";
			cin >> login;

			cout << "������� ������: ";
			cin >> password;

			cout << "������� ���: ";
			cin.ignore();
			getline(cin, name);

			chat.registerUser(login, password, name);
		}
		else if (choice == 2) {
			cout << "������� �����: ";
			cin >> login;

			cout << "������� ������: ";
			cin >> password;

			chat.login(login, password);
		}
		else if (choice == 3) {
			cout << "������..." << endl;
			break;
		}
		else {
			cout << "�������� �����! ��������� �����." << endl;
		}
	}

	//tyt
	// ������ ���������� ������ �� ������
	char sendbuf[2048] = "from client:hello server.";
	if (send(clientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
	{
		cout << "�� ������� ��������� ���������!";
	}

	// ������ ��������� ������ � �������
	char  recvbuf[2048] = {};
	if (recv(clientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
	{
		cout << "�� ������� ������� ������" << endl;
	}
	else
		cout << "������, ���������� �������� : " << recvbuf << endl;

	// ��������� �����
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}

	// ���������
	WSACleanup();
	cout << "������ ������!" << endl;
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