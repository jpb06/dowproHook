#pragma once

#include <string>
#include <vector>
#include <memory>
#include "guiddef.h"

class LuaObject;

namespace CrevetteBotApi
{
	std::wstring ParseGameResultParameters(std::unique_ptr<LuaObject> parsedGameResult, std::string hash);
	GUID SendIdentity(std::wstring guid);
	bool SendGameResult(std::wstring json);
	bool SendReplays(std::vector<std::wstring> filesPath);
}