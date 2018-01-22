#pragma once

#include <string>

class Socket
{
public:
	Socket(std::string ipAddress, int port);
	~Socket();

	bool SendString(std::string target, std::wstring json);
	bool SendFile(std::string target, std::wstring filePath);

private:
	unsigned int socketFD;

	std::string hostName;
	std::string ipAddress;
	int port;

	void Initialize();
	void Connect();
	void Close();
};