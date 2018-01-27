#include "GameResult.hpp"

#include "./../StaticAssets.hpp"
#include "GamePlayer.hpp"

GameResult::GameResult(std::wstring filePath)
{
	std::wstring content = StaticAssets::SoulstormFiles.GetGameResult(filePath);


}

GameResult::~GameResult()
{
	size_t size = this->players.size();
	for (unsigned int i = 0; i < size; i++) 
	{
		delete(this->players[i]);
	}
}


