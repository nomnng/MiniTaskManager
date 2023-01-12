#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <vector>

#include "Window.h"

using namespace std;

class ListBox : public Window {
	public:
		ListBox(HWND parent, int x, int y, int width, int height);
		void AddItem(LPWSTR str);
		void AddItems(vector<LPWSTR> items);
		void ClearItems();
		void ReplaceItem(LPWSTR str, int index);
		void SetScrollPosition();
	private:
		int m_ItemCounter = 0;
		int m_ScrollOffset = 0;
		LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnScroll(WPARAM wParam, LPARAM lParam);
};
