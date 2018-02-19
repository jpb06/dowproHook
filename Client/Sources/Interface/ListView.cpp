#include "ListView.hpp"

#include <string>
#include <CommCtrl.h>
#include ".\..\Static\StaticInterface.hpp"

#define C_COLUMNS 3

HWND ListView::CreateListView(HWND hwndParent)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;                       // The parent window's client area.

	GetClientRect(hwndParent, &rcClient);

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindow(WC_LISTVIEW,
		L"",
		WS_CHILD | LVS_REPORT | WS_VISIBLE | LVS_SHOWSELALWAYS,
		0, 0,
		rcClient.right - rcClient.left,
		230,
		hwndParent,
		NULL,//(HMENU)301,
		StaticInterface::HInstance,
		NULL);
	ListView_SetExtendedListViewStyle(hWndListView, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_LABELTIP);

	// Add some columns to the list view control
	LVCOLUMN lvc = { 0 };
	ListView_InsertColumn(hWndListView, 0, &lvc);

	lvc.mask = LVCF_TEXT;
	lvc.iSubItem++;
	std::wstring firstName(L"File Name");
	lvc.pszText = &firstName[0];
	ListView_InsertColumn(hWndListView, 1, &lvc);

	lvc.iSubItem++;
	std::wstring lastName(L"Map Name");
	lvc.pszText = &lastName[0];
	ListView_InsertColumn(hWndListView, 2, &lvc);

	lvc.iSubItem++;
	std::wstring company(L"Duration");
	lvc.pszText = &company[0];
	ListView_InsertColumn(hWndListView, 3, &lvc);

	lvc.iSubItem++;
	std::wstring winners(L"Races");
	lvc.pszText = &winners[0];
	ListView_InsertColumn(hWndListView, 4, &lvc);

	// Set column widths
	ListView_SetColumnWidth(hWndListView, 0, LVSCW_AUTOSIZE_USEHEADER);
	ListView_SetColumnWidth(hWndListView, 1, LVSCW_AUTOSIZE_USEHEADER);
	ListView_SetColumnWidth(hWndListView, 2, 150);
	ListView_SetColumnWidth(hWndListView, 3, LVSCW_AUTOSIZE_USEHEADER);
	ListView_SetColumnWidth(hWndListView, 4, 460);

	// Here's where we can add the checkbox to the column header
	// First, we need to snag the header control and give it the
	// HDS_CHECKBOXES style since the list view doesn't do this for us
	HWND header = ListView_GetHeader(hWndListView);
	DWORD dwHeaderStyle = ::GetWindowLong(header, GWL_STYLE);
	dwHeaderStyle |= HDS_CHECKBOXES;
	::SetWindowLong(header, GWL_STYLE, dwHeaderStyle);

	// Store the ID of the header control so we can handle its notification by ID
	this->headerId = ::GetDlgCtrlID(header);

	// Now, we can update the format for the first header item,
	// which corresponds to the first column
	HDITEM hdi = { 0 };
	hdi.mask = HDI_FORMAT;
	Header_GetItem(header, 0, &hdi);
	hdi.fmt |= HDF_CHECKBOX | HDF_FIXEDWIDTH;
	Header_SetItem(header, 0, &hdi);

	this->listView = hWndListView;

	return (hWndListView);
}

void ListView::CheckAllItems(BOOL fChecked) {
	for (int nItem = 0; nItem < ListView_GetItemCount(this->listView); nItem++) {
		ListView_SetCheckState(this->listView, nItem, fChecked);
	}
}

void ListView::SetHeaderCheckbox() {
	// Loop through all of our items.  If any of them are
	// unchecked, we'll want to uncheck the header checkbox.
	BOOL fChecked = TRUE;
	for (int nItem = 0; nItem < ListView_GetItemCount(this->listView); nItem++) {
		if (!ListView_GetCheckState(this->listView, nItem)) {
			fChecked = FALSE;
			break;
		}
	}

	// We need to get the current format of the header
	// and set or remove the HDF_CHECKED flag
	HWND header = ListView_GetHeader(this->listView);
	HDITEM hdi = { 0 };
	hdi.mask = HDI_FORMAT;
	Header_GetItem(header, 0, &hdi);
	if (fChecked) {
		hdi.fmt |= HDF_CHECKED;
	}
	else {
		hdi.fmt &= ~HDF_CHECKED;
	}
	Header_SetItem(header, 0, &hdi);
}

void ListView::AddItem(GameResult* gameResult)
{
	LVITEM lvi = { 0 };

	// Insert the item itself
	// Since we're always inserting item 0, new items will appear on top
	ListView_InsertItem(this->listView, &lvi);

	// Insert the subitems (columns)
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	std::wstring fileName = gameResult->GetFileName();
	lvi.pszText = &fileName[0];
	ListView_SetItem(this->listView, &lvi);

	lvi.iSubItem++;
	std::wstring mapName = gameResult->GetMapName();
	lvi.pszText = &mapName[0];
	ListView_SetItem(this->listView, &lvi);

	lvi.iSubItem++;
	std::wstring duration = gameResult->GetFormattedDuration();
	lvi.pszText = &duration[0];
	ListView_SetItem(this->listView, &lvi);

	lvi.iSubItem++;
	std::wstring races = gameResult->GetRaces();
	lvi.pszText = &races[0];
	ListView_SetItem(this->listView, &lvi);
}