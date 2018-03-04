#pragma once

#include <string>
#include <atomic>
#include <windows.h>

struct ReplayWatchTaskConfiguration {
	HWND hwnd;

	std::atomic<bool> keepRunning;

	std::wstring* playbackPath;
	std::wstring* replayFilePath;

	std::wstring* resultsPath;

	std::wstring* lastSavedFileHashPath;
	std::wstring* savedFilesListPath;

	std::wstring* archivePath;
};