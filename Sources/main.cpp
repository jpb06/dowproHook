#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include "./DowPro/ReplaysWatcher.hpp"
#include "StaticAssets.hpp"
#include "Util\FileUtil.hpp"



using namespace std;

void init()
{
	wstring playbackPath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"Playback";

	// init : first temp.req shouldn't be saved
	int fileSize = FileUtil::GetFileSize(playbackPath + L"\\temp.rec");
	FileUtil::WriteInteger(playbackPath + L"\\dowprohook.log", fileSize);

	ReplaysWatcher replaysWatcher(playbackPath);
	replaysWatcher.Start();
}

int main()
{
	//init();

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	SOCKET sock;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "Error 01: creating socket failed!\n";
		closesocket(sock);
		return 1;
	}

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);

	int error = connect(sock, (sockaddr*)&sin, sizeof(sin));
	if (error != 0)
	{
		cout << "Error 02: conecting to server failed!\n";
		closesocket(sock);
		return 1;
	}

	const char * header = "GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
	send(sock, header, strlen(header), 0);

	char answ[1024];
	while (recv(sock, answ, 1024, 0) != 0)
		cout << answ << endl;

	WSACleanup();
	
	cin.get();
	return 0;
}