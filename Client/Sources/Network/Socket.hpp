#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <sstream>

using namespace std;

class Socket
{
public:
	Socket(string ipAddress, int port);
	~Socket();

	bool SendFile(wstring filePath);

private:
	WSADATA WSAData;
	SOCKET socketFD;

	string hostName;
	string ipAddress;
	int port;

	bool Initialize();
	bool Connect();
	void Close();
};