#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include ".\DowPro\ReplaysWatcher.hpp"
#include ".\StaticAssets.hpp"
#include ".\Util\FileUtil.hpp"
#include ".\Crypto\picosha2.hpp"
#include ".\Util\IdentityUtil.hpp"

using namespace std;

wstring init()
{
	wstring ssRoot = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory();

	string guid = FileUtil::ReadString(ssRoot + L"dph_idty.dat");
	if (guid.size() == 0) 
	{
		guid = IdentityUtil::GenerateOne();

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

	while(replaysWatcher.IsRunning())
		this_thread::sleep_for(chrono::milliseconds(20000));

	return 0;
}