#include "FileUtil.hpp"

#include <stdio.h>
#include <string>
#include <fstream>
#include ".\StringUtil.hpp"
#include ".\..\Crypto\picosha2.hpp"

namespace FileUtil
{
	bool IsFileOpened(std::wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "a", _SH_DENYWR)) != NULL)
		{
			fclose(stream);
			return false;
		}

		return true;
	}

	bool FileExists(std::wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "r", _SH_DENYNO)) != NULL)
		{
			fclose(stream);
			return true;
		}

		return false;
	}

	long GetFileSize(std::wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "r", _SH_DENYWR)) != NULL)
		{
			fseek(stream, 0L, SEEK_END);
			long size = ftell(stream);
			fclose(stream);
			return size;
		}

		return -1;
	}

	bool WriteFile(std::wstring path, std::wstring content)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(path).c_str(), "w", _SH_DENYWR)) != NULL)
		{
			fprintf(stream, "%s", StringUtil::ConvertToNarrow(content).c_str());
			fclose(stream);
		}

		return true;
	}

	void Write(std::wstring file, long integer)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "w", _SH_DENYWR)) != NULL)
		{
			fprintf(stream, "%ld", integer);
			fclose(stream);
		}
	}

	void Write(std::wstring file, std::string data)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "w", _SH_DENYWR)) != NULL)
		{
			fprintf(stream, "%s", data.c_str());
			fclose(stream);
		}
	}

	long ReadInteger(std::wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "r", _SH_DENYWR)) != NULL)
		{
			long integer = 0;
			fscanf_s(stream, "%ld", &integer);
			fclose(stream);
			return integer;
		}

		return 0;
	}

	std::string ReadString(std::wstring file)
	{
		std::ifstream stream(file);
		if (stream)
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();

			return buffer.str();
		}
		return "";
	}

	int Copy(std::wstring sourceFilePath, std::wstring newFilePath)
	{
		FILE *fold, *fnew;
		errno_t err = 0, err1 = 0;
		int c;

		// Open the source file for reading in binary mode
		// fopen(oldname, "rb"), using a secure version
		err = fopen_s(&fold, StringUtil::ConvertToNarrow(sourceFilePath).c_str(), "rb");
		err1 = fopen_s(&fnew, StringUtil::ConvertToNarrow(newFilePath).c_str(), "wb");

		if (err != 0)
			return -1;

		// Open the destination file for writing in binary mode
		if (err1 != 0)
		{
			fclose(fold);
			return -1;
		}

		// Read one byte at a time from the source, if end of file has not been reached, write the byte to the destination
		while (1)
		{
			c = fgetc(fold);
			if (!feof(fold))
				fputc(c, fnew);
			else
				break;
		}

		fclose(fnew);
		fclose(fold);

		return 0;

	}

	std::string FileUtil::CalculateSHA256(std::wstring filePath)
	{
		std::ifstream fileStream(StringUtil::ConvertToNarrow(filePath), std::ios_base::in | std::ios_base::binary);
		std::string hash = picosha2::hash256_hex_string(std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()));
		return hash;
	}
}