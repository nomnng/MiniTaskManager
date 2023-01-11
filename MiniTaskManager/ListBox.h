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
};
