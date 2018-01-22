#include "GamePlayer.hpp"

GamePlayer::GamePlayer(std::wstring raceIdentifier, std::wstring name, int team, int human, int state)
{
	this->race = this->GetRace(raceIdentifier);
	this->name = this->name;
	this->team = team + 1;
	this->isAmongWinners = state == 5;
	this->isHuman = human == 1;
}

GamePlayer::~GamePlayer()
{}

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
