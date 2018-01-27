#pragma once

#include <string>
#include <vector>

namespace FileUtil
{
	bool IsFileOpened(std::wstring file);
	bool FileExists(std::wstring file);
	long GetFileSize(std::wstring file);
	void Write(std::wstring file, long integer);
	void Write(std::wstring file, std::string data);
	void Append(std::wstring file, std::string data);
	void Append(std::wstring file, std::wstring data);
	long ReadInteger(std::wstring file);
	std::string ReadString(std::wstring file);
	std::vector<std::wstring> ReadLines(std::wstring file);
	int Copy(std::wstring sourceFilePath, std::wstring newFilePath);
	void Delete(std::wstring file);
	std::string CalculateSHA256(std::wstring filePath);
}