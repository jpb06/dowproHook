#include "Socket.hpp"

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <sstream>
#include ".\..\Errors\SocketError.hpp"
#include ".\httpHeaders.hpp"
#include ".\Http.hpp"
#include ".\..\Util\StringUtil.hpp"
#include ".\..\Static\StaticAssets.hpp"
#include ".\..\Static\StaticUserData.hpp"

Socket::Socket(std::string ipAddress, int port)
{
	this->ipAddress = ipAddress;
	this->port = port;

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
}

Socket::~Socket()
{
	WSACleanup();
}

void Socket::Initialize()
{
	if ((this->socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		closesocket(this->socketFD);
		throw SocketError{ "Socket initialization failure" };
	}
}

void Socket::Connect()
{
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->port);
	inet_pton(AF_INET, this->ipAddress.c_str(), &sin.sin_addr);

	int error = connect(this->socketFD, (sockaddr*)&sin, sizeof(sin));
	if (error != 0)
	{
		closesocket(this->socketFD);
		throw SocketError{ "Socket connection failure" };
	}
}

void Socket::Close()
{
	closesocket(this->socketFD);
}

bool Socket::SendString(std::string target, std::wstring data)
{
	this->Initialize();
	this->Connect();

	std::stringstream out;
	out << "POST " + target + " HTTP/1.1\r\n";
	out << "Host: " + this->hostName + "\r\n";
	out << "Content-Length: " << data.size() << "\r\n";
	out << "\r\n";
	out << StringUtil::ConvertToNarrow(data);
	std::string requestContent = out.str();
	send(this->socketFD, requestContent.c_str(), requestContent.size(), 0);

	char buffer[1024];
	int n = 0;
	if ((n = recv(this->socketFD, buffer, sizeof buffer - 1, 0)) < 0)
	{
		this->Close();
		return false;
	}
	buffer[n] = '\0';
	std::string response(buffer);

	this->Close();

	httpHeaders headers = Http::ParseHeaders(response);
	if (headers.statusCode == 200)
		return true;
	else
		return false;
}

bool Socket::SendFile(std::string target, std::wstring filePath)
{
	this->Initialize();
	this->Connect();

	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	std::streamoff size = file.tellg();

	std::stringstream out;
	out << "POST " + target + " HTTP/1.1\r\n";
	out << "Host: " + this->hostName + "\r\n";
	out << "Content-Length: " << size << "\r\n";
	out << "Identity: " << StaticUserData::Identity << "\r\n";
	out << "\r\n";
	std::string requestHeaders = out.str();
	send(this->socketFD, requestHeaders.c_str(), requestHeaders.size(), 0);

	char data[1024];
	file.clear();
	file.seekg(0, std::ios::beg);
	while (size > 0)
	{
		int currRead = size >= 1024 ? 1024 : (int)size;
		file.read(data, currRead);
		send(this->socketFD, data, currRead, 0);

		size -= currRead;
	}
	file.close();

	char buffer[1024];
	int n = 0;
	if ((n = recv(this->socketFD, buffer, sizeof buffer - 1, 0)) < 0)
	{
		this->Close();
		return false;
	}
	buffer[n] = '\0';
	std::string response(buffer);

	this->Close();

	httpHeaders headers = Http::ParseHeaders(response);
	if (headers.statusCode == 200)
		return true;
	else
		return false;
}
