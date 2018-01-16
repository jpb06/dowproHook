#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include ".\..\StaticAssets.hpp"
#include ".\..\Util\FileUtil.hpp"
#include ".\..\Util\DateUtil.hpp"
#include ".\..\Util\StringUtil.hpp"

using namespace std;

class ReplaysWatcher
{
public:
	ReplaysWatcher(wstring playbackPath);
	~ReplaysWatcher();

	void Start();
	void Stop();
	
private:
	wstring playbackPath;
	wstring jsonFilePath;
	wstring lastSavedFileSizePath;
	wstring replayFilePath;
	wstring archivePath;

	atomic<bool> running;
	thread taskThread;
	
	void Task(atomic<bool>& program_is_running);
};