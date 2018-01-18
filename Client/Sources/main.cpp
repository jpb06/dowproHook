#include "Network/Socket.hpp"
#include <iostream>
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
	FileUtil::WriteFile(L"E:\\XenoCid\\Anno_2k18\\dowproHook\\test.json", parsedGameResult->ToJson());

	wstring archivePath = L"E:\\XenoCid\\Anno_2k18\\dowproHook\\test.zip";
	vector<wstring> filesToArchive = 
	{ 
		L"E:\\SteamGames\\steamapps\\common\\Dawn of War Soulstorm\\Playback\\stupidkasrkins.rec", 
		L"E:\\XenoCid\\Anno_2k18\\dowproHook\\test.json" 
	};
	StaticAssets::SoulstormFiles.ArchiveGame(archivePath, filesToArchive);

	Socket socket("127.0.0.1", 8080);
	bool result = socket.SendFile(archivePath);
	
	return 0;
}