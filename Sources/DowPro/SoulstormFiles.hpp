#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "./../Registry/Registry.hpp"
#include "./../Zipping/Zipper.hpp"
#include "./../Util/StringUtil.hpp"

using namespace std;

class SoulstormFiles
{
public:
	SoulstormFiles();
	~SoulstormFiles();
	
	wstring GetSelectedProfileDirectory();
	wstring GetGameResult();
	wstring GetGameResult(wstring path);
	void ArchiveGame(wstring archivePath, vector<wstring> filesPath);

	// getters
	wstring GetSoulstormRootDirectory();
private: 
	wstring soulstormRootDirectory;

	wstring ReadTextFile(wstring path);
};