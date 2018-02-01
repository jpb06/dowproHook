#pragma once

#include <string>
#include <vector>
#include <memory>
#include ".\..\Lua\LuaObject.hpp"

class GamePlayer;

class GameResult
{
public:
	GameResult(std::unique_ptr<LuaObject> parsedGameResult, std::wstring fileName);
	~GameResult();

	std::wstring GetFileName();
	std::wstring GetMapName();
	std::wstring GetFormattedDuration();
	std::wstring GetWinCondition();
	std::wstring GetWinners();
	std::wstring GetPlayersAndRacesAsLines();
	int GetPlayersCount();

private:
	std::wstring fileName;
	std::wstring mapName;
	std::wstring winCondition;
	int teamsCount;
	int duration;
	int playersCount;
	std::vector<std::unique_ptr<GamePlayer>> players;
};