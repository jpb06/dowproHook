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

	// lauching soulstorm
	system("start steam://rungameid/9450");
	
	return 0;
}