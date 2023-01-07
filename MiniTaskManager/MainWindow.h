#pragma once
#include <Windows.h>

class MainWindow {
public:
	MainWindow();
	void ProcessMessages();

private:
	bool RegisterWndClass(HINSTANCE hInstance);
	bool CreateWnd(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	const LPCWSTR m_WindowName = L"MiniTaskManager";
	const LPCWSTR m_ClassName = L"MiniTaskMgrClass";
	HWND m_WindowHandle;
};
