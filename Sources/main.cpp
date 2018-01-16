#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <sstream>
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

	wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
	unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);
	FileUtil::WriteFile(L"E:\\XenoCid\\Anno_2k18\\test.json", parsedGameResult->ToJson());

	wstring archivePath = L"E:\\XenoCid\\Anno_2k18\\test.zip";
	vector<wstring> filesToArchive = 
	{ 
		L"E:\\SteamGames\\steamapps\\common\\Dawn of War Soulstorm\\Playback\\stupidkasrkins.rec", 
		L"E:\\XenoCid\\Anno_2k18\\test.json" 
	};
	StaticAssets::SoulstormFiles.ArchiveGame(archivePath, filesToArchive);

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
		cout << "Error 02: connecting to server failed!\n";
		closesocket(sock);
		return 1;
	}

	ifstream file(archivePath, ios::binary | ios::ate);
	streamoff size = file.tellg();

	stringstream out;
	out << "POST /SendResult HTTP/1.1\r\n";
	out << "Host: localhost:8080\r\n";
	out << "Content-Length: " << size << "\r\n";
	out << "\r\n";
	string headers = out.str();
	send(sock, headers.c_str(), headers.size(), 0);

	char data[1024];
	file.clear();
	file.seekg(0, ios::beg);
	while (size > 0)
	{
		int currRead = size >= 1024 ? 1024 : (int)size;
		file.read(data, currRead);
		send(sock, data, currRead, 0);

		size -= currRead;
	}
	file.close();

	char answ[1024];
	while (recv(sock, answ, 1024, 0) != 0)
		cout << answ << endl;

	WSACleanup();
	
	return 0;
}