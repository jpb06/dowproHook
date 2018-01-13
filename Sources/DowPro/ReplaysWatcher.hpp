#pragma once

#include <string>
#include "SoulstormFiles.hpp"

using namespace std;

class ReplaysWatcher
{
public:
	ReplaysWatcher(wstring playbackPath);
	~ReplaysWatcher();

	void Start();
	
private:
	wstring playbackPath;
};