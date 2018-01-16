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

void SoulstormFiles::ArchiveGame(const wstring archivePath, vector<wstring> filesPath)
{
	ziputils::zipper zipFile;
	zipFile.open(StringUtil::ConvertToNarrow(archivePath).c_str());

	for(unsigned int i = 0; i < filesPath.size(); i++)
	{
		string fileName = StringUtil::ConvertToNarrow(filesPath[i]);
		string extension = fileName.substr(fileName.find_last_of('.'));

		ifstream file(fileName, ios::in | ios::binary);
		if (file.is_open())
		{
			const char* entryFileName = "";
			if (extension == ".json") entryFileName = "/GameResult.json";
			if (extension == ".rec") entryFileName = "/Replay.rec";

			// TODO : exception on empty name

			zipFile.addEntry(entryFileName);
			zipFile << file;
		}
		file.close();
	}

	zipFile.close();
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