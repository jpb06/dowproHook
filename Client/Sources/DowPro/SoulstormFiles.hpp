#pragma once

#include <string>
#include <vector>

using namespace std;

class SoulstormFiles
{
public:
	SoulstormFiles();
	~SoulstormFiles();
	
	wstring GetSelectedProfileDirectory();
	wstring GetGameResult();
	wstring GetGameResult(wstring path);
	void ArchiveFiles(wstring archivePath, vector<wstring> filesPath);

	// getters
	wstring GetSoulstormRootDirectory();
private: 
	wstring soulstormRootDirectory;

	wstring ReadTextFile(wstring path);
};