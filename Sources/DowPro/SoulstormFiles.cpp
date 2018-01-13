#include "SoulstormFiles.hpp"

SoulstormFiles::SoulstormFiles()
{
	wstring soulstormRootDirectory = Registry::RegGetString(
		HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Wow6432Node\\THQ\\Dawn of War - Soulstorm", 
		L"installlocation"
	);
	this->soulstormRootDirectory = soulstormRootDirectory + L"\\";
}

SoulstormFiles::~SoulstormFiles()
{}

wstring SoulstormFiles::GetSoulstormRootDirectory()
{
	return soulstormRootDirectory;
}

wstring SoulstormFiles::GetSelectedProfileDirectory()
{
	wstring profileData = this->ReadTextFile(this->soulstormRootDirectory + L"Local.ini");

	size_t first = profileData.find(L"playerprofile=") + 14;
	size_t last = profileData.find(L"\n", first);
	wstring profile = profileData.substr(first, last - first);

	return this->soulstormRootDirectory + L"Profiles\\" + profile + L"\\";
}

wstring SoulstormFiles::GetGameResult()
{
	wstring path = this->GetSelectedProfileDirectory() + L"testStats.Lua";
	return this->GetGameResult(path);
}

wstring SoulstormFiles::GetGameResult(wstring path)
{
	wstring gameResultData = this->ReadTextFile(path);

	return gameResultData;
}

wstring SoulstormFiles::ReadTextFile(wstring path)
{
	ifstream ifs(path);
	wstring content(
		(istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>())
	);
	ifs.close();

	return content;
}