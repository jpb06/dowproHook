#pragma once

#include <windows.h>
#include <string>
#include ".\..\Interface\ListView.hpp"

class StaticInterface
{
public:
	static std::wstring ClassName;
	static HINSTANCE HInstance;
	static HFONT MainFont;
	static HFONT BoldFont;
	static ListView ListviewControl;
};