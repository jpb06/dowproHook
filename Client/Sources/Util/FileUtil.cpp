#include "FileUtil.hpp"

namespace FileUtil
{
	bool IsFileOpened(wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "a", _SH_DENYWR)) != NULL)
		{
			fclose(stream);
			return false;
		}

		return true;
	}

	bool FileExists(wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "r", _SH_DENYNO)) != NULL)
		{
			fclose(stream);
			return true;
		}

		return false;
	}

	long GetFileSize(wstring file)
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

	bool WriteFile(wstring path, wstring content)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(path).c_str(), "w", _SH_DENYWR)) != NULL)
		{
			fprintf(stream, "%s", StringUtil::ConvertToNarrow(content).c_str());
			fclose(stream);
		}

		return true;
	}

	void WriteInteger(wstring file, long integer)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "w", _SH_DENYWR)) != NULL)
		{
			fprintf(stream, "%ld", integer);
			fclose(stream);
		}
	}

	long ReadInteger(wstring file)
	{
		FILE *stream;

		if ((stream = _fsopen(StringUtil::ConvertToNarrow(file).c_str(), "r", _SH_DENYWR)) != NULL)
		{
			long* integer = 0;
			fscanf_s(stream, "%ld", integer);
			fclose(stream);
			return *integer;
		}

		return 0;
	}

	int Copy(wstring sourceFilePath, wstring newFilePath)
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
}