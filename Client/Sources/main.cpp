#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <windows.h>
#include ".\DowPro\ReplaysWatcher.hpp"
#include ".\StaticAssets.hpp"
#include ".\Util\FileUtil.hpp"
#include ".\Errors\ApplicationError.hpp"
#include ".\Util\StringUtil.hpp"
#include ".\Crypto\picosha2.hpp"

using namespace std;

wstring init()
{
	wstring ssRoot = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory();

	string guid = FileUtil::ReadString(ssRoot + L"dph_idty.dat");
	if (guid.size() == 0) 
	{
		GUID guidReference;
		HRESULT hCreateGuid = CoCreateGuid(&guidReference);

		if (hCreateGuid != 0)
			throw ApplicationError("Couldn't generate a guid");

		wchar_t szGUID[64] = { 0 };
		StringFromGUID2(guidReference, szGUID, 64);

		guid = StringUtil::ConvertToNarrow(wstring(szGUID));

		FileUtil::Write(ssRoot + L"dph_idty.dat", guid);
	}

	StaticAssets::Identity = guid;

	// init : first temp.req shouldn't be saved
	ifstream ifs(ssRoot + L"\\Playback\\temp.rec", ios_base::in | ios_base::binary);
	string hash = picosha2::hash256_hex_string(string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()));
	ifs.close();
	FileUtil::Write(ssRoot + L"\\dph_lahsh.dat", hash);

	return ssRoot;
}

void launchSoulstorm()
{
	// lauching soulstorm
	system("start steam://rungameid/9450");
}

int main()
{
	wstring ssRootPath = init();
	ReplaysWatcher replaysWatcher(ssRootPath);
	replaysWatcher.Start();

	while(replaysWatcher.isRunning())
		this_thread::sleep_for(chrono::milliseconds(20000));

	return 0;
}