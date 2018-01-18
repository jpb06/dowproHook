#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <sstream>
#include "./../Errors/SocketError.hpp"
#include "./httpHeaders.hpp"
#include "./Http.hpp"

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

	void Initialize();
	void Connect();
	void Close();
};