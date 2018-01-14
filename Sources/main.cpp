#include <iostream>
#include "./DowPro/ReplaysWatcher.hpp"

#include <vector>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#include "StaticAssets.hpp"


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

				FileUtil::WriteFile(playBackPath + L"\\dowprohook_r.json", parsedGameResult->ToJson());
				vector<wstring> filesToArchive = { playBackPath + L"\\" + filename, playBackPath + L"\\dowprohook_r.json" };
				StaticAssets::SoulstormFiles.ArchiveGame(playBackPath + L"\\dowprohook.zip", filesToArchive);
			}
		}
		this_thread::sleep_for(wait_duration);
		cout << "- looping" << endl;
	}
}

int main()
{
	wstring playbackPath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"Playback";
	
	// init : first temp.req shouldn't be saved
	int fileSize = FileUtil::GetFileSize(playbackPath + L"\\temp.rec");
	FileUtil::WriteInteger(playbackPath + L"\\dowprohook.log", fileSize);

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