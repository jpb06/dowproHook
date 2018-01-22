#pragma once

#include <string>

namespace FileUtil
{
	bool IsFileOpened(std::wstring file);
	bool FileExists(std::wstring file);
	long GetFileSize(std::wstring file);
	bool WriteFile(std::wstring path, std::wstring content);
	void Write(std::wstring file, long integer);
	void Write(std::wstring file, std::string data);
	long ReadInteger(std::wstring file);
	std::string ReadString(std::wstring file);
	int Copy(std::wstring sourceFilePath, std::wstring newFilePath);
	std::string CalculateSHA256(std::wstring filePath);
}