#pragma once

#include <string>
#include <fstream>
#include "./../Registry/Registry.hpp"

using namespace std;

class SoulstormFiles
{
public:
	SoulstormFiles();
	~SoulstormFiles();
	
	wstring GetSelectedProfileDirectory();
	wstring GetGameResult();
	wstring GetGameResult(wstring path);

	// getters
	wstring GetSoulstormRootDirectory();
private: 
	wstring soulstormRootDirectory;

	wstring ReadTextFile(wstring path);
};