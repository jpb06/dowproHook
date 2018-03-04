#pragma once

#include <Windows.h>
#include <string>
#include ".\..\DowPro\GameResult.hpp"

class ListView
{
private:
	HWND listView;
	int headerId;

	
public:
	HWND CreateListView(HWND hwndParent);
	void AddItem(GameResult* gameResult);

	void CheckAllItems(BOOL fChecked);
	void SetHeaderCheckbox();
    std::vector<int> GetSelectedItemsIndexes();

	void ClearItems();
	void RemoveSelectedItems();
	void RemoveItem(int index);

	void Refresh();
};