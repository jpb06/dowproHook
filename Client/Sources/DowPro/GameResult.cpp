#include "GameResult.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include ".\..\Static\StaticAssets.hpp"
#include ".\..\Lua\LuaProperty.hpp"
#include "GamePlayer.hpp"

GameResult::GameResult(std::unique_ptr<LuaObject> parsedGameResult, std::wstring fileName)
{
	this->duration = parsedGameResult->Get<LuaProperty>(L"Duration")->AsInt();
	this->teamsCount = parsedGameResult->Get<LuaProperty>(L"Teams")->AsInt();
	this->playersCount = unsigned(parsedGameResult->Get<LuaProperty>(L"Players")->AsInt());
	
	this->winCondition = parsedGameResult->Get<LuaProperty>(L"WinBy")->AsString();
	this->mapName = parsedGameResult->Get<LuaProperty>(L"Scenario")->AsString();
	
	this->fileName = fileName;

	for(unsigned int i = 0; i < this->playersCount; i++)
	{
		const LuaObject* playerData = parsedGameResult->Get<LuaObject>(L"player_"+ std::to_wstring(i));
		std::unique_ptr<GamePlayer> player(new GamePlayer(playerData));
		this->players.push_back(move(player));
	}
}

GameResult::~GameResult()
{}

std::wstring GameResult::GetFileName()
{
	return this->fileName;
}

std::wstring GameResult::GetMapName()
{
	return this->mapName;
}

std::wstring GameResult::GetFormattedDuration()
{
	int hours = this->duration / 3600;
	int seconds = this->duration % 3600;
	int minutes = seconds / 60;
	seconds %= 60;

	std::wstringstream wss;
	wss << std::setfill(L'0') << std::setw(2) << hours << ":"
		<< std::setfill(L'0') << std::setw(2) << minutes << ":"
		<< std::setfill(L'0') << std::setw(2) << seconds;
	std::wstring time = wss.str();

	return time;
}

std::wstring GameResult::GetWinCondition()
{
	return this->winCondition;
}

std::wstring GameResult::GetWinners()
{
	std::wstring winners;

	for(unsigned int i = 0; i < this->players.size(); i++)
	{
		GamePlayer* player = this->players.at(i).get();
		if (player->IsAmongWinners())
			winners += player->GetName() + L", ";
	}

	return winners.substr(0, winners.size() - 2);
}

std::wstring GameResult::GetPlayersAndRacesAsLines()
{
	std::wstring lines;

	for (unsigned int i = 0; i < this->players.size(); i++)
	{
		GamePlayer* player = this->players.at(i).get();
		lines += player->GetName() + L" ( " + player->GetRace() + L" )\n";
	}

	return lines;
}

unsigned int GameResult::GetPlayersCount()
{
	return this->playersCount;
}

std::wstring GameResult::GetRaces()
{
	std::wstring races;

	for (unsigned int i = 0; i < this->players.size(); i++)
	{
		GamePlayer* player = this->players.at(i).get();
		std::wstring race = player->GetRace();

		size_t pos = races.find(race);
		if(pos == std::string::npos)
		{
			races += race + L", ";
		}
	}

	return races.substr(0, races.size() - 2);
}

GamePlayer * GameResult::GetPlayer(unsigned int index)
{
	if (index >= this->playersCount) return NULL;

	GamePlayer* player = this->players.at(index).get();
	return player;
}
