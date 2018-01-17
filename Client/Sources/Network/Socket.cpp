#include "Socket.hpp"

Socket::Socket(string ipAddress, int port)
{
	this->ipAddress = ipAddress;
	this->port = port;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
}

Socket::~Socket()
{
	WSACleanup();
}

bool Socket::Initialize()
{
	if ((this->socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		closesocket(this->socketFD);
		return false;
	}

	return true;
}

bool Socket::Connect()
{
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->port);
	inet_pton(AF_INET, this->ipAddress.c_str(), &sin.sin_addr);

	int error = connect(this->socketFD, (sockaddr*)&sin, sizeof(sin));
	if (error != 0)
	{
		closesocket(this->socketFD);
		return false;
	}

	return true;
}

void Socket::Close()
{
	closesocket(this->socketFD);
}

bool Socket::SendFile(wstring filePath)
{
	this->Initialize();
	this->Connect();

	ifstream file(filePath, ios::binary | ios::ate);
	streamoff size = file.tellg();

	stringstream out;
	out << "POST /SendResult HTTP/1.1\r\n";
	out << "Host: " + this->hostName + "\r\n";
	out << "Content-Length: " << size << "\r\n";
	out << "\r\n";
	string headers = out.str();
	send(this->socketFD, headers.c_str(), headers.size(), 0);

	char data[1024];
	file.clear();
	file.seekg(0, ios::beg);
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
	string response(buffer);
	cout << response;

	this->Close();

	return true;
}
