#include "SavedReplay.hpp"

#include ".\GameResult.hpp"

SavedReplay::SavedReplay(std::wstring path, std::wstring recFileName, std::wstring gameResultFileName)
{
	this->path = path;
	this->recFileName = recFileName;
	this->gameResultFileName = gameResultFileName;

	//this->gameResult = new GameResult(path + gameResultFileName, recFileName);
}

SavedReplay::~SavedReplay()
{
	delete(this->gameResult);
}

std::wstring SavedReplay::GetRecFilePath()
{
	return this->path + this->recFileName;
}

std::wstring SavedReplay::GetGameResultFilePath()
{
	return this->path + this->gameResultFileName;
}

bool SavedReplay::Erase()
{
	return false;
}
