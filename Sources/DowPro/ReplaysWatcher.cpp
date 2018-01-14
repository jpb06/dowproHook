#include "ReplaysWatcher.hpp"

ReplaysWatcher::ReplaysWatcher(wstring playbackPath)
{
	this->playbackPath = playbackPath;
	this->jsonFilePath = playbackPath + L"\\dowprohook_r.json";
	this->lastSavedFileSizePath = playbackPath + L"\\dowprohook.log";
	this->replayFilePath = playbackPath + L"\\temp.rec";
	this->archivePath = playbackPath + L"\\dowprohook.zip";

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

void ReplaysWatcher::Task(atomic<bool>& program_is_running)
{
	const auto wait_duration = chrono::milliseconds(20000);

	while (program_is_running)
	{
		bool isOpened = FileUtil::IsFileOpened(this->replayFilePath);
		if (!isOpened)
		{
			long lastSavedFileSize = FileUtil::ReadInteger(this->lastSavedFileSizePath);
			long fileSize = FileUtil::GetFileSize(this->replayFilePath);

			if (fileSize != lastSavedFileSize)
			{
				wstring rawGameResult = StaticAssets::SoulstormFiles.GetGameResult();
				unique_ptr<LuaObject> parsedGameResult = StaticAssets::Lua.ParseObject(rawGameResult);

				wstring mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
				string formattedDate = DateUtil::GetCurrentFormattedTime();

				wstring filename = mapName + L'_' + StringUtil::ConvertToWide(formattedDate) + L".rec";
				StringUtil::RemoveIllegalCharacters(&filename);

				FileUtil::Copy(this->replayFilePath, this->playbackPath + L"\\" + filename);
				FileUtil::WriteInteger(this->lastSavedFileSizePath, fileSize);

				FileUtil::WriteFile(this->jsonFilePath, parsedGameResult->ToJson());
				vector<wstring> filesToArchive = { this->replayFilePath, this->jsonFilePath };
				StaticAssets::SoulstormFiles.ArchiveGame(this->archivePath, filesToArchive);
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
