#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "Window.h"
#include "ListView.h"
#include "ProcessEnumerator.h"

using namespace std;

#define MAIN_WINDOW_CLASS_NAME TEXT("MiniTaskMgrClass")
#define TIMER_ID1 1

class MainWindow : Window {
public:
	MainWindow(LPCWSTR windowName, int width, int height);
	void MessageLoop();

private:
	LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnResize(WPARAM wParam, LPARAM lParam);
	void OnTimer(WPARAM wParam);
	void UpdateProcessList();

	ListView *m_ProcessListView;
};
