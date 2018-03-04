#include "ReplaysWatcher.hpp"

#include <string>
#include <thread>
#include <chrono>
#include <process.h>
#include <fstream>

#include ".\..\Lua\LuaProperty.hpp"
#include ".\..\Crypto\picosha2.hpp"
#include ".\..\Network\Api\CrevetteBotApi.hpp"
#include ".\..\Static\StaticAssets.hpp"
#include ".\..\Static\StaticUserData.hpp"
#include ".\..\Util\FileUtil.hpp"
#include ".\..\Util\DateUtil.hpp"
#include ".\..\Util\StringUtil.hpp"
#include ".\..\Util\ProcessUtil.hpp"
#include ".\..\Tasking\ReplayWatchTaskConfiguration.hpp"

#include ".\..\Static\StaticInterface.hpp"
#include ".\..\..\resource.h"

unsigned int __stdcall ReplaysWatcher::Task(void* pArgument)
{
	ReplayWatchTaskConfiguration* pConfiguration = static_cast<ReplayWatchTaskConfiguration*>(pArgument);

	const auto waitDuration = std::chrono::milliseconds(5000);

	while (pConfiguration->keepRunning)
	{
		bool isSoulstormRunning = ProcessUtil::IsProcessRunning(L"Soulstorm.exe");
		if (isSoulstormRunning)
		{
			bool isOpened = FileUtil::IsFileOpened(*pConfiguration->replayFilePath);
			if (!isOpened)
			{
				std::string lastSavedFileHash = FileUtil::ReadString(*pConfiguration->lastSavedFileHashPath);
				std::ifstream ifs(*pConfiguration->replayFilePath, std::ios_base::in | std::ios_base::binary);
				std::string hash = picosha2::hash256_hex_string(std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
				ifs.close();

				// New file
				if (hash != lastSavedFileHash)
				{
					// Get related game result
					std::wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
					std::unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);

					// Get replay save name content = mapname_date.rec
					std::wstring mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
					std::string formattedDate = DateUtil::GetCurrentFormattedTime();

					std::wstring wideFormattedDate = StringUtil::ConvertToWide(formattedDate);
					std::wstring filename = mapName + std::wstring(L"_") + std::wstring(wideFormattedDate.c_str()) + L".rec";
					StringUtil::RemoveIllegalCharacters(&filename);

					// copy replay file 
					FileUtil::Copy(*pConfiguration->replayFilePath, (*pConfiguration->playbackPath) + filename);
					FileUtil::Write(*pConfiguration->lastSavedFileHashPath, hash);

					// parse game result to json and save it (using replay hash as filename)
					std::wstring gameResultJson = parsedGameResult->ToJson();
					FileUtil::Write(*(pConfiguration->resultsPath) + StringUtil::ConvertToWide(hash) + L".json", StringUtil::ConvertToNarrow(gameResultJson));

					// add saved replay to the upload list
					//FileUtil::Append(*pConfiguration->savedFilesListPath, (*pConfiguration->playbackPath) + filename);

					// Add replay to the UI listview
					std::unique_ptr<GameResult> gameResult(new GameResult(move(parsedGameResult), filename));
					StaticUserData::GameResults.insert(StaticUserData::GameResults.begin(), std::move(gameResult));
					SendMessage(pConfiguration->hwnd, WM_LISTVIEW_ADDITEM, (WPARAM)StaticUserData::GameResults.size() - 1, NULL);
				}
			}
		}

		std::this_thread::sleep_for(waitDuration);
	}

	_endthreadex(0);
	return 0;
}