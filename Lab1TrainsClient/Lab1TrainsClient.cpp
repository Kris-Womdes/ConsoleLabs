//This project files includes
#include "resource.h"

//WinAPI includes
#include <WinSock2.h>
#include <WS2tcpip.h>

//C++ standart includes
#include <iostream>
#include <regex>
#include <string>
#include <thread>

int ec{ 0 }; //error code

#define fl_args __FILE__, __LINE__, "Not supported yet"
//show custom error msg
void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, const char* pErrorDefinition)
{
	std::cout
		<< "Welcome to hell of errors!"
		<< "\nFile: " << fileName
		<< "\nLine: " << lineNum
		<< "\nError code: " << codePart
		<< "\nReason: " << pErrorDefinition
		<< "\n\n";
}
//show error msg by code
void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, DWORD ec)
{
	char* pErrorDefinition;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		ec,
		0,
		(LPSTR)&pErrorDefinition,
		0,
		nullptr
	);
	ShowEM(fileName, lineNum, codePart, pErrorDefinition);
	LocalFree(pErrorDefinition);
}

std::ostream& operator<<(std::ostream& ostr, WSADATA wsadata)
{
	return ostr
		<< "Description: " << wsadata.szDescription
		<< "\nVersion (initialized): " << (unsigned short int)HIBYTE(wsadata.wVersion) << '.' << (unsigned short int)LOBYTE(wsadata.wVersion)
		<< "\nHigh version (the highest version available on the system): " << (unsigned short int)HIBYTE(wsadata.wHighVersion) << '.' << (unsigned short int)LOBYTE(wsadata.wHighVersion)
		<< "\nSystem status: " << wsadata.szSystemStatus
		//<< "\nVendor info: " << WSAData.lpVendorInfo
		<< "\nMaxUdpDg: " << wsadata.iMaxUdpDg
		<< "\nMaxSockets: " << wsadata.iMaxSockets
		<< "\n\n";
};

bool isValidIpAddress(const std::string& ip) {
	struct sockaddr_in sa;
	return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
}
bool isValidPort(const std::string& portStr) {
	int port = std::stoi(portStr);
	return port > 0 && port <= 65535;
}

void waitEsc()
{
	std::cout << "To exit, Esc is possible.\n\n";
	while (!(GetAsyncKeyState(VK_ESCAPE) & 1))
	{
		Sleep(100);
	}
}
bool wsaIsStarted{ 0 };
SOCKET sockClient{ INVALID_SOCKET };
bool sockClientIsCreated{ 0 };
bool isConnected{ 0 };
//escape key monitor
void escmon()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 1)
		{
			//let server know client exit
			if (isConnected)
			{
				send(sockClient, "H9d5v6v4bB5D6H554BJ7F907443", 28, 0);
			}

			if (sockClientIsCreated && closesocket(sockClient) == SOCKET_ERROR)
			{
				MessageBeep(MB_ICONHAND);
				ec = WSAGetLastError();
				ShowEM(fl_args, ec);
				waitEsc();
			}

			if (wsaIsStarted && WSACleanup() == SOCKET_ERROR)
			{
				MessageBeep(MB_ICONHAND);
				ec = WSAGetLastError();
				ShowEM(fl_args, ec);
				waitEsc();
			}
			ExitProcess(ec);
			break;
		}
		Sleep(100);
	}
}
std::thread escthr(escmon);
void waitExit()
{
	std::cout << "To exit, Esc is possible.\n\n";
	escthr.join();
}

int main(int argc, char* argv[]) //если было несколько ошибок, то вернёт последнюю
{
	//debug mode?
	bool d{ 0 };
	for (int i{ 0 }; i < argc - 1; i++)
	{
		d = !std::strcmp(argv[i], "d");
	}

	//title for... what?
	SetConsoleTitleA("");

	HINSTANCE hInstance = GetModuleHandleA(0);
	HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
	HWND hwnd = GetConsoleWindow();
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessageA(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	WSADATA WSAData;
	ec = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (ec)
	{
		MessageBeep(MB_ICONHAND);
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	wsaIsStarted = 1;
	if (d) { std::cout << WSAData; }

	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	sockClientIsCreated = 1;
	if (d) { std::cout << "Client socket descriptor: " << sockClient << "\n\n"; }

	sockaddr_in sockAddrInServer;
	sockAddrInServer.sin_family = AF_INET;
	//getting ip:port
	{
		std::regex ipPort_Pattern(R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}):(\d+))");
		std::cout << "Enter the server IP-address:port here: ";
		std::string input;
		std::getline(std::cin, input);
		std::cout << '\n';
		std::smatch matches;
		while (true)
		{
			if (std::regex_match(input, matches, ipPort_Pattern))
			{
				std::string ipStr = matches[1];
				std::string portStr = matches[2];
				if (isValidIpAddress(ipStr) && isValidPort(portStr))
				{
					inet_pton(AF_INET, ipStr.c_str(), &sockAddrInServer.sin_addr.S_un.S_addr);
					sockAddrInServer.sin_port = htons(std::stoi(portStr));
					break;
				}
				else
				{
					MessageBeep(MB_ICONHAND);
					std::cout << "The entered IP-address:port is not correct. Ask server admin for help.\n\n";
				}
			}
			else
			{
				MessageBeep(MB_ICONHAND);
				std::cout << "The input format is not valid. XXX.XXX.XXX.XXX:XXXXX format is expected. Ask server admin for help.\n\n";
			}
			std::cout << "Try again: ";
			std::getline(std::cin, input);
			std::cout << "\n";
		}
	}

	if (connect(sockClient, (sockaddr*)&sockAddrInServer, sizeof(sockAddrInServer)) == SOCKET_ERROR)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	isConnected = 1;

	std::string msg;
	char buffer[1024];
	int bytesReceived;
	while (1)
	{
		std::cout << "Enter : ";
		std::getline(std::cin, msg);
		std::cout << '\n';
		send(sockClient, msg.c_str(), msg.size(), 0);
		std::string fullAnswer;
		while ((bytesReceived = recv(sockClient, buffer, sizeof(buffer), 0)) > 0)
		{
			fullAnswer.append(buffer, bytesReceived);
			if (bytesReceived < sizeof(buffer)) { break; }
		}
		std::cout << fullAnswer << "\n\n";
	}
	return ec;
}