#pragma once

#include <string>
#include <vector>

class GamePlayer;

class GameResult
{
public:
	GameResult(std::wstring filePath);
	~GameResult();

private:
	std::wstring winCondition;
	std::wstring teamsCount;
	std::wstring playersCount;
	std::vector<GamePlayer*> players;
};