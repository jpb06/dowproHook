#pragma once

#include <string>
#include "GameResult.hpp"

using namespace std;

class SavedReplay
{
public:
	SavedReplay(wstring path, wstring recFileName, wstring gameResultFileName);
	~SavedReplay();

	wstring GetRecFilePath();
	wstring GetGameResultFilePath();

	bool Erase();

private:
	wstring path;
	wstring recFileName;
	wstring gameResultFileName;
	GameResult* gameResult;
};