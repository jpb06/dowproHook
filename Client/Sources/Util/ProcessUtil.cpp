#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

namespace ProcessUtil
{
	/*!
	\brief Check if a process is running
	\param [in] processName Name of process to check if is running
	\returns \c True if the process is running, or \c False if the process is not running
	*/
	bool IsProcessRunning(const wchar_t *processName)
	{
		bool exists = false;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry))
		{
			while (Process32Next(snapshot, &entry))
			{
				if (!_wcsicmp(entry.szExeFile, processName))
				{
					exists = true;
					break;
				}
			}
		}

		CloseHandle(snapshot);
		return exists;
	}
}