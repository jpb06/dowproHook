#pragma once

#include <string>
#include ".\..\Lua\LuaObject.hpp"

class GamePlayer
{
public:
	GamePlayer(std::wstring raceIdentifier, std::wstring name, int team, int human, int state);
	GamePlayer(const LuaObject* player);
	~GamePlayer();

	bool IsHuman();
	std::wstring GetRace();
	std::wstring GetName();
	int GetTeam();
	bool IsAmongWinners();

private:
	bool isHuman;
	std::wstring race;
	std::wstring name;
	int team;
	bool isAmongWinners;

	std::wstring GetRace(std::wstring raceIdentifier);
};