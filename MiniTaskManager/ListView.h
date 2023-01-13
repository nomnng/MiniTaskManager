#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <vector>

#include "Window.h"

using namespace std;

class ListView : public Window {
	public:
		ListView(HWND parent, int x, int y, int width, int height);
		void AddColumn(LPTSTR name, int width, int minWidth);	
		void AddItem(vector<LPTSTR> item);
		void DeleteAllItems();
	private:
		LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		int m_ColumnCount = 0;
		int m_ItemCount = 0;
};
