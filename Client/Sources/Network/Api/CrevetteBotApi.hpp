#pragma once

#include <string>
#include <vector>
#include <memory>

class LuaObject;

namespace CrevetteBotApi
{
	std::wstring ParseGameResultParameters(std::unique_ptr<LuaObject> parsedGameResult, std::string hash);
	bool SendGameResult(std::wstring json);
	bool SendReplays(std::vector<std::wstring> filesPath);
}