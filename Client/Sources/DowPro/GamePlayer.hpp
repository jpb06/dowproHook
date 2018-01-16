#pragma once

#include <string>

using namespace std;

class GamePlayer
{
public:
	GamePlayer(wstring raceIdentifier, wstring name, int team, int human, int state);
	~GamePlayer();



private:
	bool isHuman;
	wstring race;
	wstring name;
	int team;
	bool isAmongWinners;

	wstring GetRace(wstring raceIdentifier);
};