#include "ReplaysWatcher.hpp"

#include <chrono>
#include <fstream>
#include ".\..\Lua\LuaProperty.hpp"
#include ".\..\Lua\LuaObject.hpp"
#include ".\..\Crypto\picosha2.hpp"
#include ".\..\Network\Api\CrevetteBotApi.hpp"
#include ".\..\StaticAssets.hpp"
#include ".\..\Util\FileUtil.hpp"
#include ".\..\Util\DateUtil.hpp"
#include ".\..\Util\StringUtil.hpp"

ReplaysWatcher::ReplaysWatcher(std::wstring soulstormPath)
{
	this->playbackPath = soulstormPath + L"Playback";
	this->lastSavedFileHashPath = soulstormPath + L"dph_lahsh.dat";
	this->replayFilePath = soulstormPath + L"Playback\\temp.rec";
	this->archivePath = soulstormPath + L"dph_garc.zip";

	this->running = false;
}

ReplaysWatcher::~ReplaysWatcher()
{
	if (this->taskThread.joinable())
	{
		this->running = false;
		this->taskThread.detach();
		this->~ReplaysWatcher();
	}
}

void ReplaysWatcher::Task(std::atomic<bool>& program_is_running)
{
	const auto wait_duration = chrono::milliseconds(20000);

	while (program_is_running)
	{
		bool isOpened = FileUtil::IsFileOpened(this->replayFilePath);
		if (!isOpened)
		{
			std::string lastSavedFileHash = FileUtil::ReadString(this->lastSavedFileHashPath);
			std::ifstream ifs(this->replayFilePath, ios_base::in | ios_base::binary);
			std::string hash = picosha2::hash256_hex_string(string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()));
			ifs.close();

			if (hash != lastSavedFileHash)
			{
				std::wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
				std::unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);

				std::wstring mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
				std::string formattedDate = DateUtil::GetCurrentFormattedTime();

				std::wstring filename = mapName + L'_' + StringUtil::ConvertToWide(formattedDate) + L".rec";
				StringUtil::RemoveIllegalCharacters(&filename);

				FileUtil::Copy(this->replayFilePath, this->playbackPath + L"\\" + filename);
				FileUtil::Write(this->lastSavedFileHashPath, hash);

				std::wstring gameResultJson = parsedGameResult->ToJson();
				std::wstring guid = StringUtil::ConvertToWide(StaticAssets::Identity);
				std::wstring requestJson = L"{\"Identity\":\"" + guid.substr(1, guid.size()-2) + L"\", \"GameResult\":" + gameResultJson + L"}";
				
				CrevetteBotApi::SendGameResult(requestJson);
			}
		}
		this_thread::sleep_for(wait_duration);
	}
}

void ReplaysWatcher::Start()
{
	this->running = true;
	this->taskThread = thread(&ReplaysWatcher::Task, this, ref(running));
}

void ReplaysWatcher::Stop()
{
	this->running = false;
	this->taskThread.join();
}

bool ReplaysWatcher::isRunning()
{
	return this->running;
}
