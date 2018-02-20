#include "ReplaysWatcher.hpp"

#include <chrono>
#include <fstream>
#include ".\..\Lua\LuaProperty.hpp"
#include ".\..\Lua\LuaObject.hpp"
#include ".\..\Crypto\picosha2.hpp"
#include ".\..\Network\Api\CrevetteBotApi.hpp"
#include ".\..\Static\StaticAssets.hpp"
#include ".\..\Static\StaticUserData.hpp"
#include ".\..\Util\FileUtil.hpp"
#include ".\..\Util\DateUtil.hpp"
#include ".\..\Util\StringUtil.hpp"
#include ".\..\Util\ProcessUtil.hpp"

ReplaysWatcher::ReplaysWatcher(std::wstring soulstormPath)
{
	this->playbackPath = soulstormPath + L"Playback";
	this->lastSavedFileHashPath = soulstormPath + L"dph_lahsh.dat";
	this->replayFilePath = soulstormPath + L"Playback\\temp.rec";
	this->archivePath = soulstormPath + L"dph_garc.zip";
	this->savedFilesListPath = soulstormPath + L"dph_sf.dat";

	this->running = false;
}

ReplaysWatcher::~ReplaysWatcher()
{
	if (this->taskThread.joinable())
	{
		this->running = false;
		this->taskThread.detach();
	}
}

void ReplaysWatcher::Task(std::atomic<bool>& keepRunning)
{
	const auto waitDuration = std::chrono::milliseconds(20000);

	while (keepRunning)
	{
		bool isSoulstormRunning = ProcessUtil::IsProcessRunning(L"Soulstorm.exe");
		if(isSoulstormRunning)
		{
			bool isOpened = FileUtil::IsFileOpened(this->replayFilePath);
			if (!isOpened)
			{
				std::string lastSavedFileHash = FileUtil::ReadString(this->lastSavedFileHashPath);
				std::ifstream ifs(this->replayFilePath, std::ios_base::in | std::ios_base::binary);
				std::string hash = picosha2::hash256_hex_string(std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
				ifs.close();

				if (hash != lastSavedFileHash)
				{
					std::wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
					std::unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);

					std::wstring mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
					std::string formattedDate = DateUtil::GetCurrentFormattedTime();

					std::wstring wideFormattedDate = StringUtil::ConvertToWide(formattedDate);
					std::wstring filename = mapName + std::wstring(L"_") + std::wstring(wideFormattedDate.c_str()) + L".rec";
					StringUtil::RemoveIllegalCharacters(&filename);

					FileUtil::Copy(this->replayFilePath, this->playbackPath + L"\\" + filename);
					FileUtil::Write(this->lastSavedFileHashPath, hash);

					std::wstring gameResultJson = parsedGameResult->ToJson();
					std::wstring guid = StringUtil::ConvertToWide(StaticUserData::Identity);
					std::wstring requestJson = L"{\"Identity\":\"" + guid.substr(1, guid.size() - 2) + L"\", \"GameResult\":" + gameResultJson + L"}";

					FileUtil::Append(this->savedFilesListPath, this->playbackPath + L"\\" + filename);

					bool result = CrevetteBotApi::SendGameResult(requestJson);
					int a = 0;
					// if failed, save result to send it later
				}
			}
		}
		else 
		{
			std::vector<std::wstring> filesPaths = FileUtil::ReadLines(this->savedFilesListPath);
			if(filesPaths.size() > 0)
			{
				FileUtil::Delete(this->savedFilesListPath);

				bool result = CrevetteBotApi::SendReplays(filesPaths);
				// if failed, save result to send it later
			}
		}

		std::this_thread::sleep_for(waitDuration);
	}
}

void ReplaysWatcher::Start()
{
	this->running = true;
	this->taskThread = std::thread(&ReplaysWatcher::Task, this, ref(running));
}

void ReplaysWatcher::Stop()
{
	this->running = false;
	this->taskThread.join();
}

bool ReplaysWatcher::IsRunning()
{
	return this->running;
}
