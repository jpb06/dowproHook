#pragma once

#include <string>

class GamePlayer
{
public:
	GamePlayer(std::wstring raceIdentifier, std::wstring name, int team, int human, int state);
	~GamePlayer();



private:
	bool isHuman;
	std::wstring race;
	std::wstring name;
	int team;
	bool isAmongWinners;

	std::wstring GetRace(std::wstring raceIdentifier);
};