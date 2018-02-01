#include "GamePlayer.hpp"

#include ".\..\Lua\LuaProperty.hpp"

GamePlayer::GamePlayer(std::wstring raceIdentifier, std::wstring name, int team, int human, int state)
{
	this->race = this->GetRace(raceIdentifier);
	this->name = this->name;
	this->team = team + 1;
	this->isAmongWinners = state == 5;
	this->isHuman = human == 1;
}

GamePlayer::GamePlayer(const LuaObject* player)
{
	this->race = this->GetRace(player->Get<LuaProperty>(L"PRace")->AsString());
	this->name = player->Get<LuaProperty>(L"PName")->AsString();
	this->team = player->Get<LuaProperty>(L"PTeam")->AsInt() + 1;
	this->isAmongWinners = player->Get<LuaProperty>(L"PFnlState")->AsInt() == 5;
	this->isHuman = player->Get<LuaProperty>(L"PHuman")->AsInt() == 1;
}

GamePlayer::~GamePlayer()
{}

bool GamePlayer::IsHuman()
{
	return this->isHuman;
}

std::wstring GamePlayer::GetRace()
{
	return this->race;
}

std::wstring GamePlayer::GetName()
{
	return this->name;
}

int GamePlayer::GetTeam()
{
	return this->team;
}

bool GamePlayer::IsAmongWinners()
{
	return this->isAmongWinners;
}

std::wstring GamePlayer::GetRace(std::wstring raceIdentifier)
{
	if(raceIdentifier == L"chaos_marine_race")
		return L"Chaos";
	if (raceIdentifier == L"eldar_race")
		return L"Eldar";
	if (raceIdentifier == L"ork_race")
		return L"Orks";
	if (raceIdentifier == L"space_marine_race")
		return L"Space marines";
	if (raceIdentifier == L"guard_race")
		return L"Imperial guard";
	if (raceIdentifier == L"necron_race")
		return L"Necrons";
	if (raceIdentifier == L"tau_race")
		return L"Tau";
	if (raceIdentifier == L"sisters_race")
		return L"Sisters of battle";
	if (raceIdentifier == L"dark_eldar_race")
		return L"Dark Eldar";

	return nullptr;
}
