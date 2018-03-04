#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <iostream>
#include <thread>
//#include <chrono>
#include <fstream>

#include ".\Tasking\ReplaysWatcher.hpp"
#include ".\Tasking\ReplayWatchTaskConfiguration.hpp"
#include <process.h>  

#include ".\Static\StaticAssets.hpp"
#include ".\Static\StaticUserData.hpp"
#include ".\Static\StaticInterface.hpp"
#include ".\Static\StaticApplicationLogicData.hpp"
#include ".\Util\FileUtil.hpp"
#include ".\Crypto\picosha2.hpp"
#include ".\Util\IdentityUtil.hpp"


#include <windows.h>
#include <CommCtrl.h>
#include <memory>
#include <string>
#include "./DowPro/GameResult.hpp"
#include "./DowPro/GamePlayer.hpp"
#include "../resource.h"


#include "./Interface/ListView.hpp"
#include "./Util/StringUtil.hpp"

#include ".\Interface\ResourcesHandling.hpp"

#include <strsafe.h>

using namespace std;

wstring init()
{
	std::wstring ssRoot = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory();

	std::wstring hookFolder = ssRoot + L"hook\\";
	std::wstring resultsFolder = hookFolder + L"\\results\\";
	CreateDirectory(hookFolder.c_str(), NULL); 
	CreateDirectory(resultsFolder.c_str(), NULL);

	string guid = FileUtil::ReadString(hookFolder + L"dph_idty.dat");
	if (guid.size() == 0)
	{
		guid = IdentityUtil::GenerateOne();

		FileUtil::Write(hookFolder + L"dph_idty.dat", guid);
	}

	StaticUserData::Identity = guid;

	// init : first temp.req shouldn't be saved
	ifstream ifs(ssRoot + L"\\Playback\\temp.rec", ios_base::in | ios_base::binary);
	string hash = picosha2::hash256_hex_string(string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()));
	ifs.close();
	FileUtil::Write(hookFolder + L"\\dph_lahsh.dat", hash);

	return ssRoot;
}

void launchSoulstorm()
{
	// lauching soulstorm
	system("start steam://rungameid/9450");
}


int selectedGameResult = -1;

unsigned int threadID;
void*  hThread;
ReplayWatchTaskConfiguration config;

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		HWND listView = StaticInterface::ListviewControl.CreateListView(hwnd);

		for (unsigned int i = 0; i < StaticUserData::GameResults.size(); i++)
		{
			GameResult* result = StaticUserData::GameResults.at(i).get();
			StaticInterface::ListviewControl.AddItem(result);
		}

		config.hwnd = hwnd;

		config.keepRunning = true;
		config.playbackPath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"Playback\\");
		config.replayFilePath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"Playback\\temp.rec");
		config.resultsPath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"hook\\results\\");

		config.lastSavedFileHashPath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"hook\\dph_lahsh.dat");
		config.archivePath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"hook\\dph_garc.zip");
		config.savedFilesListPath = new std::wstring(StaticApplicationLogicData::SoulstormPath + L"hook\\dph_sf.dat");
		
		hThread = (HANDLE)_beginthreadex(NULL, 0, &ReplaysWatcher::Task, (void*)&config, 0, &threadID);

		return 0;
	}
	case WM_LISTVIEW_ADDITEM:
	{
		int index = (int)wParam;
		GameResult* result = StaticUserData::GameResults.at(index).get();

		StaticInterface::ListviewControl.AddItem(result);
		StaticInterface::ListviewControl.Refresh();

		return 0;
	}
	case WM_NOTIFY:
	{
		LPNMHDR lpnHdr = (LPNMHDR)lParam;
		switch (lpnHdr->code)
		{
			case LVN_ITEMCHANGED:
			{
				LPNMLISTVIEW pnmlv = (LPNMLISTVIEW)lpnHdr;

				if (pnmlv->uChanged & LVIF_STATE)
				{
					StaticInterface::ListviewControl.SetHeaderCheckbox();
				}

				BOOL bSelectedNow = (pnmlv->uNewState & LVIS_SELECTED);
				BOOL bSelectedBefore = (pnmlv->uOldState  & LVIS_SELECTED);
				
				if(!bSelectedBefore && bSelectedNow)
				{
				    int lvSelectedIndex = pnmlv->iItem;
					int gameResultsIndex = StaticUserData::GameResults.size() - lvSelectedIndex - 1;
					selectedGameResult = gameResultsIndex;

					RECT rcClient;                       // The parent window's client area.
					GetClientRect(hwnd, &rcClient);
					rcClient.top = 230;
					InvalidateRect(hwnd, &rcClient, TRUE);
				}
				
				return 0;
			}
			case HDN_ITEMSTATEICONCLICK:
			{
				LPNMHEADER header = (LPNMHEADER)lpnHdr;

				if (header->pitem->mask & HDI_FORMAT && header->pitem->fmt & HDF_CHECKBOX)
				{
					StaticInterface::ListviewControl.CheckAllItems(!(header->pitem->fmt & HDF_CHECKED));
					StaticInterface::ListviewControl.SetHeaderCheckbox();
				}
				break;
			}
		}
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_MENU_SEND_ALL:

			break;
		case ID_MENU_SEND_SEL:

			break;
		case ID_MENU_DISCARD_ALL: 
		{
			StaticUserData::GameResults.clear();
			StaticInterface::ListviewControl.ClearItems();
			break; 
		}
		case ID_MENU_DISCARD_SEL: 
		{
			std::vector<std::shared_ptr<GameResult>> clearedGameResults;
			std::vector<int> indexes = StaticInterface::ListviewControl.GetSelectedItemsIndexes();

			for(unsigned int i = 0; i < StaticUserData::GameResults.size(); i++)
			{
				if (std::find(indexes.begin(), indexes.end(), i) == indexes.end())
				{
					clearedGameResults.push_back(std::move(StaticUserData::GameResults.at(i)));
				}
			}

			StaticUserData::GameResults.clear();
			
			StaticUserData::GameResults = clearedGameResults;

			StaticInterface::ListviewControl.ClearItems();
			for (int j = StaticUserData::GameResults.size() - 1; j >= 0; j--)
			{
				GameResult* result = StaticUserData::GameResults.at(j).get();
				StaticInterface::ListviewControl.AddItem(result);
			}
			break; 
		}
		case ID_MENU_HELP:
			MessageBox(hwnd, L"Clic", L"Bonjour.", MB_ICONINFORMATION);
			break;
		case ID_MENU_QUIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}

		return 0;
	}
	case WM_PAINT:
	{
		if(selectedGameResult != -1)
		{
			GameResult* gameResult = StaticUserData::GameResults.at(selectedGameResult).get();

			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			std::string path("E:\\XenoCid\\Anno_2k18\\dowproHook\\maps\\" + StringUtil::ConvertToNarrow(gameResult->GetMapName()) + ".jpg");
			HBITMAP hBitmap = (HBITMAP)ResourcesHandling::LoadImage(&path[0]);

			HDC hdcMem = CreateCompatibleDC(ps.hdc);
			HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

			BITMAP bitmap;
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			BitBlt(ps.hdc, 10, 240, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

			RECT rc;
			rc.left = 10;
			rc.top = 370;
			rc.right = 138;
			rc.bottom = 400;
			
			std::wstring mapName = gameResult->GetMapName(); 
			HFONT hFontOld = (HFONT)SelectObject(ps.hdc, StaticInterface::MainFont);
			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, &mapName[0], mapName.size(), &rc, DT_CENTER | DT_TOP | DT_WORDBREAK);
			SelectObject(ps.hdc, hFontOld);

			GetClientRect(hwnd, &rc);
			rc.left = 148;
			rc.top = 240;

			std::wstring playersHeader = L"Players";
			hFontOld = (HFONT)SelectObject(ps.hdc, StaticInterface::MainFont);
			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, &playersHeader[0], playersHeader.size(), &rc, DT_LEFT | DT_TOP);
			SelectObject(ps.hdc, hFontOld);

			MoveToEx(ps.hdc, 148, 257, NULL);
			LineTo(ps.hdc, 400, 257);

			int pos = 260;

			for(unsigned int i = 0; i<gameResult->GetPlayersCount(); i++)
			{
				rc.top = pos;
				rc.bottom = pos+16;

				GamePlayer* player = gameResult->GetPlayer(i);
				std::wstring playerDesc = player->GetName() + L" - " + player->GetRace();
				if (player->IsAmongWinners()) playerDesc += L" (Wins)";
				SetBkMode(ps.hdc, TRANSPARENT);

				HFONT f;
				if (player->IsAmongWinners())
					f = StaticInterface::BoldFont;
				else
					f = StaticInterface::MainFont;

				HFONT hFontOld = (HFONT)SelectObject(ps.hdc, f);
				DrawText(ps.hdc, &playerDesc[0], playerDesc.size(), &rc, DT_LEFT | DT_TOP);
				SelectObject(ps.hdc, hFontOld);

				pos += 17;
			}

			SelectObject(hdcMem, oldBitmap);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);

			EndPaint(hwnd, &ps);
		}
		
		return 0;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:

		DeleteObject(StaticInterface::MainFont);
		DeleteObject(StaticInterface::BoldFont);

		config.keepRunning = false;

		WaitForSingleObject(hThread, INFINITE);

		delete(config.archivePath);
		delete(config.lastSavedFileHashPath);
		delete(config.resultsPath);
		delete(config.playbackPath);
		delete(config.replayFilePath);
		delete(config.savedFilesListPath);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StaticInterface::ClassName = L"DowPro hook";
	StaticInterface::MainFont = CreateFont(16, 0, 0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Tahoma"
	);
	StaticInterface::BoldFont = CreateFont(16, 0, 0, 0,
		FW_SEMIBOLD,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Tahoma"
	);

	StaticApplicationLogicData::SoulstormPath = init();

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	StaticInterface::HInstance = hInstance;

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX); // specifying size
	wc.cbClsExtra = 0; // Additional Memory Request
	wc.cbWndExtra = 0; // Window Extra-Memory
	wc.style = 0; // CS_HREDRAW | CS_VREDRAW // redrawing when resizing
	wc.lpfnWndProc = WndProc; // Message Processing
	wc.hInstance = hInstance; // communicate with winMain : make instance available as a resource
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);//COLOR_WINDOWFRAME);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // window menu
	wc.lpszClassName = StaticInterface::ClassName.c_str();


	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	// Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		StaticInterface::ClassName.c_str(),
		L"DoWpro Hook",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//  Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}