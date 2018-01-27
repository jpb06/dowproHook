#include "SoulstormFiles.hpp"

#include <fstream>
#include "./../Registry/Registry.hpp"
#include "./../Zipping/Zipper.hpp"
#include "./../Util/StringUtil.hpp"

SoulstormFiles::SoulstormFiles()
{
	std::wstring soulstormRootDirectory = Registry::RegGetString(
		HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Wow6432Node\\THQ\\Dawn of War - Soulstorm", 
		L"installlocation"
	);
	this->soulstormRootDirectory = soulstormRootDirectory + L"\\";
}

SoulstormFiles::~SoulstormFiles()
{}

std::wstring SoulstormFiles::GetSoulstormRootDirectory()
{
	return soulstormRootDirectory;
}

std::wstring SoulstormFiles::GetSelectedProfileDirectory()
{
	std::wstring profileData = this->ReadTextFile(this->soulstormRootDirectory + L"Local.ini");

	size_t first = profileData.find(L"playerprofile=") + 14;
	size_t last = profileData.find(L"\n", first);
	std::wstring profile = profileData.substr(first, last - first);

	return this->soulstormRootDirectory + L"Profiles\\" + profile + L"\\";
}

std::wstring SoulstormFiles::GetGameResult()
{
	std::wstring path = this->GetSelectedProfileDirectory() + L"testStats.Lua";
	return this->GetGameResult(path);
}

std::wstring SoulstormFiles::GetGameResult(std::wstring path)
{
	std::wstring gameResultData = this->ReadTextFile(path);

	return gameResultData;
}

void SoulstormFiles::ArchiveFiles(const std::wstring archivePath, std::vector<std::wstring> filesPath)
{
	ziputils::zipper zipFile;
	zipFile.open(StringUtil::ConvertToNarrow(archivePath).c_str());

	for(unsigned int i = 0; i < filesPath.size(); i++)
	{
		std::string filePath = StringUtil::ConvertToNarrow(filesPath[i]);

		std::ifstream file(filePath, std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			size_t lastSlash = filePath.rfind('\\');
			std::string fileName = filePath.substr(lastSlash + 1);
			zipFile.addEntry(fileName.c_str());
			zipFile << file;
		}
		file.close();
	}

	zipFile.close();
}

std::wstring SoulstormFiles::ReadTextFile(std::wstring path)
{
	std::ifstream ifs(path);
	std::wstring content(
		(std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>())
	);
	ifs.close();

	return content;
}