#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <CommCtrl.h>

#include "Window.h"
#include "ListView.h"
#include "ProcessEnumerator.h"
#include "Utils.h"

using namespace std;

#define MAIN_WINDOW_CLASS_NAME TEXT("MiniTaskMgrClass")
#define TIMER_ID1 1
#define TIMER_INTERVAL_MS 1000

#define COLUMN_PID_INDEX 0
#define COLUMN_NAME_INDEX 1
#define COLUMN_CPU_INDEX 2

class MainWindow : Window {
public:
	MainWindow(LPCWSTR windowName, int width, int height);
	void MessageLoop();

private:
	LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnResize(WPARAM wParam, LPARAM lParam);
	void OnTimer(WPARAM wParam);
	void UpdateProcessList();
	void InitProcessList();
	LRESULT ProcessNotification(WPARAM wParam, LPARAM lParam);

	ListView *m_ProcessListView;
	vector<DWORD> m_ProcessIDs;
	vector<ULONG64> m_ProcessUsedCPU;
};
