#include <iostream>
#include "Registry.h"

using namespace std;

int main()
{
	Registry registry;
	DWORD data = registry.RegGetDword(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\THQ\\Dawn of War - Soulstorm", L"test");
	wstring s = registry.RegGetString(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\THQ\\Dawn of War - Soulstorm", L"installlocation");

	cout << data << endl;
	wcout << s << endl;
	cin.get();

	return 0;
}