#include <iostream>
#include "./DowPro/ReplaysWatcher.hpp"

#include <vector>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#include "StaticAssets.hpp"
#include "./Zipping/Zipper.hpp"

#include "Util\StringUtil.hpp"
#include "Util\FileUtil.hpp"
#include "Util\DateUtil.hpp"

using namespace std;

void loop(wstring playBackPath, atomic<bool>& program_is_running)
{
	const auto wait_duration = chrono::milliseconds(20000);
	while (program_is_running)
	{
		bool isOpened = FileUtil::IsFileOpened(playBackPath + L"\\temp.rec");
		if(!isOpened)
		{
			cout << "\tnot opened" << endl;
			long lastSavedFileSize = FileUtil::ReadInteger(playBackPath + L"\\dowprohook.log");
			long fileSize = FileUtil::GetFileSize(playBackPath + L"\\temp.rec");

			if(fileSize != lastSavedFileSize)
			{
				cout << "\tdifferent sizes" << endl;

				wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
				unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);

				wstring mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
				string formattedDate = DateUtil::GetCurrentFormattedTime();

				wstring filename = mapName + L'_' + StringUtil::ConvertToWide(formattedDate) + L".rec";
				StringUtil::RemoveIllegalCharacters(&filename);

				FileUtil::Copy(playBackPath + L"\\temp.rec", playBackPath + L"\\" + filename);
				FileUtil::WriteInteger(playBackPath + L"\\dowprohook.log", fileSize);
			}
		}
		this_thread::sleep_for(wait_duration);
		cout << "- looping" << endl;
	}
}

int main()
{
	//wstring playbackPath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"Playback";
	//
	//// init : first temp.req shouldn't be saved
	//int fileSize = FileUtil::GetFileSize(playbackPath + L"\\temp.rec");
	//FileUtil::WriteInteger(playbackPath + L"\\dowprohook.log", fileSize);

	const char* Filenames[] = { "E:\\Documents (Racine)\\Im@ges\\6369401_460s.jpg", "E:\\Documents (Racine)\\Documents(texts)\\Planet x Delirium.txt" };
	unsigned int nCount = sizeof(Filenames) / sizeof(char*);

	ziputils::zipper zipFile;
	zipFile.open("E:\\XenoCid\\Anno_2k18\\dowproHook\\test.zip");

	ifstream file("E:\\Documents (Racine)\\Im@ges\\6369401_460s.jpg", ios::in | ios::binary);
	if (file.is_open())
	{
		zipFile.addEntry("/6369401_460s.jpg");
		zipFile << file;
	}
	file.close();
	file.open("E:\\Documents (Racine)\\Documents(texts)\\Planet x Delirium.txt", ios::in | ios::binary);
	if (file.is_open())
	{
		zipFile.addEntry("/Planet x Delirium.txt");
		zipFile << file;
	}
	zipFile.close();

	cin.get();
	return 0;
}

//ReplaysWatcher replaysWatcher(soulstormPath + L"Playback\\");

/*wstring gameResult = StaticAssets::SoulstormFiles.GetGameResult();

unique_ptr<LuaObject> obj = StaticAssets::Lua.ParseObject(gameResult);

const LuaProperty* d = obj->Get<LuaProperty>(1);
const LuaObject* o = obj->Get<LuaObject>(L"player_0");
wstring k = d->AsString();

const LuaProperty* tt = o->Get<LuaProperty>(L"PTtlSc");

int v = tt->AsInt();*/
///////////////////////////////////////////////////////////////////////////////////////////

//atomic<bool> running{ true };
//const unsigned int update_interval = 10000; 
//thread update_thread(loop, path, ref(running), update_interval);

//// do other stuff in parallel: simulated below
//cin.get();

//// exit gracefully
//running = false;
//update_thread.join();