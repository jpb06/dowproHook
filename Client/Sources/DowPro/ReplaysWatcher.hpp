#pragma once

#include <string>
#include <atomic>
#include <thread>

class ReplaysWatcher
{
public:
	ReplaysWatcher(std::wstring soulstormPath);
	~ReplaysWatcher();

	void Start();
	void Stop();
	bool IsRunning();
	
private:
	std::wstring playbackPath;
	std::wstring jsonFilePath;
	std::wstring lastSavedFileHashPath;
	std::wstring replayFilePath;
	std::wstring archivePath;
	std::wstring savedFilesListPath;

	std::atomic<bool> running;
	std::thread taskThread;
	
	void Task(std::atomic<bool>& keepRunning);
};