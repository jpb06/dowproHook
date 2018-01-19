#include <iostream>
#include "./DowPro/ReplaysWatcher.hpp"
#include "StaticAssets.hpp"
#include "Util\FileUtil.hpp"


#include ".\Crypto\picosha2.hpp"
#include ".\Crypto\Base64.hpp"

using namespace std;

void init()
{
	wstring playbackPath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"Playback";

	// init : first temp.req shouldn't be saved
	int fileSize = FileUtil::GetFileSize(playbackPath + L"\\temp.rec");
	FileUtil::WriteInteger(playbackPath + L"\\dowprohook.log", fileSize);

	ReplaysWatcher replaysWatcher(playbackPath);
	replaysWatcher.Start();
}

void launchSoulstorm()
{
	// lauching soulstorm
	system("start steam://rungameid/9450");
}

int main()
{
	//init();

	ifstream ifs("E:\\XenoCid\\Anno_2k18\\dowproHook\\test.zip", ios_base::in | ios_base::binary);
	cout << picosha2::hash256_hex_string(string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>())) << endl;

	//--------------------------------------------------------------
	/*string src_str = "The quick brown fox jumps over the lazy dog";
	string hash_hex_str;
	picosha2::hash256_hex_string(src_str, hash_hex_str);
	cout << hash_hex_str << endl;*/

	return 0;
}

