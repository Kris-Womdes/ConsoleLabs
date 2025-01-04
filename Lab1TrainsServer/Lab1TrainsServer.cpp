//if exit, send "server has been stopped", disconnect all clients
//multiple clients: threads

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>
#include "resource.h"
#include<thread>
#include <regex>

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
		<< '\n';
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

void waitEsc()
{
	std::cout << "To exit, Esc is possible.\n\n";
	while (!(GetAsyncKeyState(VK_ESCAPE) & 1))
	{
		Sleep(100);
	}
}
bool wsaIsStarted{ 0 };
SOCKET sockServer{ INVALID_SOCKET };
bool sockServerIsCreated{ 0 };
SOCKET sockClient{ INVALID_SOCKET };
bool sockClientIsCreated{ 0 };
//escape key monitor
void escmon()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 1)
		{
			if (sockClientIsCreated && closesocket(sockClient) == SOCKET_ERROR)
			{
				MessageBeep(MB_ICONHAND);
				ec = WSAGetLastError();
				ShowEM(fl_args, ec);
				waitEsc();
			}

			if (sockServerIsCreated && closesocket(sockServer) == SOCKET_ERROR)
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

int main() //если было несколько ошибок, то вернёт последнюю
{
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
	std::cout << WSAData;

	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if (sockServer == INVALID_SOCKET)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	sockServerIsCreated = 1;
	std::cout << "Server socket descriptor: " << sockServer << "\n\n";

	sockaddr_in sockAddrInServer;
	sockAddrInServer.sin_family = AF_INET;
	sockAddrInServer.sin_addr.S_un.S_addr = INADDR_ANY;
	sockAddrInServer.sin_port = 0;
	if (bind(sockServer, (sockaddr*)&sockAddrInServer, sizeof(sockAddrInServer)) == SOCKET_ERROR)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}

	int namelenServer = sizeof(sockAddrInServer);
	if (getsockname(sockServer, (sockaddr*)&sockAddrInServer, &namelenServer) == SOCKET_ERROR)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	ULONG bufferSize = 0;
	GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &bufferSize);
	IP_ADAPTER_ADDRESSES* pAdAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufferSize);
	ec = GetAdaptersAddresses(AF_INET, 0, nullptr, pAdAddresses, &bufferSize);
	if (ec)
	{
		MessageBeep(MB_ICONHAND);
		ShowEM(fl_args, ec);
		free(pAdAddresses);
		waitExit();
		return ec;
	}
	std::cout << "Server addresses\n";
	for (IP_ADAPTER_ADDRESSES* pAd = pAdAddresses; pAd; pAd = pAd->Next)
	{
		std::cout << "\tat the interface " << pAd->AdapterName << '\n';
		for (IP_ADAPTER_UNICAST_ADDRESS* pIP = pAd->FirstUnicastAddress; pIP; pIP = pIP->Next)
		{
			char strIP[16];
			inet_ntop(AF_INET, &((sockaddr_in*)pIP->Address.lpSockaddr)->sin_addr, strIP, sizeof(strIP));
			std::cout << "\t\t" << strIP << ':' << ntohs(sockAddrInServer.sin_port) << '\n';
		}
	}
	std::cout << '\n';
	free(pAdAddresses);

	if (listen(sockServer, SOMAXCONN) == SOCKET_ERROR)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}

	sockaddr_in sockAddrInClient;
	int namelenClient = sizeof(sockAddrInClient);
	std::cout << "Listening...\n\n";
	sockClient = accept(sockServer, (sockaddr*)&sockAddrInClient, &namelenClient);
	if (sockClient == INVALID_SOCKET)
	{
		MessageBeep(MB_ICONHAND);
		ec = WSAGetLastError();
		ShowEM(fl_args, ec);
		waitExit();
		return ec;
	}
	sockClientIsCreated = 1;
	char ipClient[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sockAddrInClient.sin_addr, ipClient, INET6_ADDRSTRLEN);
	std::string ipClientStr(ipClient);
	std::string portClient = std::to_string(ntohs(sockAddrInClient.sin_port));
	std::string ipPort_Client{ ipClientStr + ':' + portClient };
	std::cout << "Client " << ipPort_Client << " connected.\n\n";

	char buffer[1024];
	int bytesReceived;
	std::string fullMsg;
	std::regex searchPattern(R"(?:(\d{1,2}:\d{2})?)");
	while (1)
	{
		fullMsg = "";
		while ((bytesReceived = recv(sockClient, buffer, sizeof(buffer), 0)) > 0)
		{
			fullMsg.append(buffer, bytesReceived);
			if (bytesReceived < sizeof(buffer)) { break; }
		}
		std::cout << "Got \"" << fullMsg << "\" from client " << ipPort_Client << "\n\n";

		const char* pCharFullMsg = fullMsg.c_str();
		std::string answer;
		if (!std::strcmp(pCharFullMsg, "H9d5v6v4bB5D6H554BJ7F907443"))
		{
			std::cout << "Client " << ipPort_Client << "disconnected.\n\n";
			if (closesocket(sockClient) == SOCKET_ERROR)
			{
				MessageBeep(MB_ICONHAND);
				ec = WSAGetLastError();
				ShowEM(fl_args, ec);
				waitEsc();
			}
			sockClientIsCreated = 0;
			break;
		}
		if (1)
		{

		}
		else
		{
			answer = "Nononnno";
		}
		send(sockClient, answer.c_str(), answer.size(), 0);
	}
	return ec;
}
