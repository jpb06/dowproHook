#pragma once

#include "./DowPro/SoulstormFiles.hpp"
#include "./Lua/Lua.hpp"
#include "./Network/Api/CrevetteBotApiConfiguration.hpp"

class StaticAssets
{
public:
	static SoulstormFiles SoulstormFiles;
	static Lua Lua;
	static CrevetteBotApiConfiguration ApiConfiguration;
	static string Identity;
};