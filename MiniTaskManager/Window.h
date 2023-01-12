#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

class Window {
	public:
		HWND GetHWND();
		void SetRect(int x, int y, int width, int height);
		void SetPosition(int x, int y);
		void SetSize(int width, int height);
	protected:
		Window(HWND parent, int x, int y, int width, int height, LPCWSTR windowName);

		bool CreateWnd(LPCWSTR className , DWORD style, HWND parent, bool subclassed);
		bool RegisterWndClass(UINT style, HICON icon, HCURSOR cursor, HBRUSH backgroundBrush, LPCWSTR className);

		static LRESULT CALLBACK RegisteredWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK SubclassedWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

		HWND m_WindowHandle;
		LPCWSTR m_ClassName;
		LPCWSTR m_WindowName;
		HWND m_Parent;
		int m_Width;
		int m_Height;
		struct {
			int x;
			int y;
		} m_Position;
		WNDPROC m_OriginalWindowProc;
};
