#pragma once

#include <stdio.h>
#include <string>
#include "StringUtil.hpp"

using namespace std;

namespace FileUtil
{
	bool IsFileOpened(wstring file);
	bool FileExists(wstring file);
	long GetFileSize(wstring file);
	bool WriteFile(wstring path, wstring content);
	void WriteInteger(wstring file, long integer);
	long ReadInteger(wstring file);
	int Copy(wstring sourceFilePath, wstring newFilePath);
}