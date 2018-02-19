#pragma once

#include <vector>
#include <memory>
#include ".\..\DowPro\GameResult.hpp"

class StaticUserData
{
public:
	static std::string Identity;
	static std::vector<std::shared_ptr<GameResult>> GameResults;
};