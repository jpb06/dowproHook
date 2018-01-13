#pragma once

#include <string>
#include <vector>
#include "GamePlayer.hpp"
#include "./../StaticAssets.hpp"

using namespace std;

class GameResult
{
public:
	GameResult(wstring filePath);
	~GameResult();

private:
	wstring winCondition;
	wstring teamsCount;
	wstring playersCount;
	vector<GamePlayer*> players;
};