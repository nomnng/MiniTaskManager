#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "Window.h"
#include "ListBox.h"

using namespace std;

class MainWindow : Window {
public:
	MainWindow(LPCWSTR windowName, int width, int height);
	void MessageLoop();

private:
	bool RegisterWndClass();
	LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnResize(WPARAM wParam, LPARAM lParam);

	const LPCWSTR m_ClassName = L"MiniTaskMgrClass";
	ListBox *m_ProcessListBox;
};
