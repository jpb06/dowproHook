#pragma once

#include <string>
#include <vector>

class SoulstormFiles
{
public:
	SoulstormFiles();
	~SoulstormFiles();
	
	std::wstring GetSelectedProfileDirectory();
	std::wstring GetGameResult();
	std::wstring GetGameResult(std::wstring path);
	void ArchiveFiles(std::wstring archivePath, std::vector<std::wstring> filesPath);

	// getters
	std::wstring GetSoulstormRootDirectory();
private: 
	std::wstring soulstormRootDirectory;

	std::wstring ReadTextFile(std::wstring path);
};