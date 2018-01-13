#include "SavedReplay.hpp"

SavedReplay::SavedReplay(wstring path, wstring recFileName, wstring gameResultFileName)
{
	this->path = path;
	this->recFileName = recFileName;
	this->gameResultFileName = gameResultFileName;

	this->gameResult = new GameResult(path + gameResultFileName);
}

SavedReplay::~SavedReplay()
{
	delete(this->gameResult);
}

wstring SavedReplay::GetRecFilePath()
{
	return this->path + this->recFileName;
}

wstring SavedReplay::GetGameResultFilePath()
{
	return this->path + this->gameResultFileName;
}

bool SavedReplay::Erase()
{
	return false;
}
