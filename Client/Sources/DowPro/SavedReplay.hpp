#pragma once

#include <string>

class GameResult;

class SavedReplay
{
public:
	SavedReplay(std::wstring path, std::wstring recFileName, std::wstring gameResultFileName);
	~SavedReplay();

	std::wstring GetRecFilePath();
	std::wstring GetGameResultFilePath();

	bool Erase();

private:
	std::wstring path;
	std::wstring recFileName;
	std::wstring gameResultFileName;
	GameResult* gameResult;
};