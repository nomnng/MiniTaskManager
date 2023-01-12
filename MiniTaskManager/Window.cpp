#include "Window.h"

HWND Window::GetHWND() {
	return m_WindowHandle;
}

Window::Window(HWND parent, int x, int y, int width, int height, LPCWSTR windowName)
	: m_Parent(parent), m_Position{x, y}, m_Width(width), m_Height(height), m_WindowName(windowName),
	m_OriginalWindowProc(0), m_WindowHandle(0), m_ClassName(0) {}

bool Window::CreateWnd(LPCWSTR className, DWORD style, HWND parent, bool subclassed) {
	m_WindowHandle = CreateWindow(
		className,
		m_WindowName,
		style,
		m_Position.x,
		m_Position.y,
		m_Width,
		m_Height,
		(HWND)parent,
		(HMENU)NULL,
		NULL,
		(LPVOID)this
	);

	if (m_WindowHandle) {
		SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, (LONG)this);
		m_OriginalWindowProc = (WNDPROC)SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, (LONG)SubclassedWindowProc);
	}

	return m_WindowHandle != NULL;
}

void Window::SetRect(int x, int y, int width, int height) {
	m_Width = width;
	m_Height = height;
	m_Position.x = x;
	m_Position.y = y;
	SetWindowPos(m_WindowHandle, 0, x, y, width, height, SWP_SHOWWINDOW | SWP_NOZORDER);
}

void Window::SetPosition(int x, int y) {
	m_Position.x = x;
	m_Position.y = y;
	SetWindowPos(m_WindowHandle, 0, x, y, 0, 0, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
}

void Window::SetSize(int width, int height) {
	m_Width = width;
	m_Height = height;
	SetWindowPos(m_WindowHandle, 0, 0, 0, width, height, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE);
}

LRESULT Window::RegisteredWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CREATE) {
		CREATESTRUCT* createInfo = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)createInfo->lpCreateParams);
	} else {
		Window* ptr = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (ptr)
			return ptr->ProcessMessage(hwnd, msg, wParam, lParam);
		else 
			return DefWindowProc(hwnd, msg, wParam, lParam); // for messages before WM_CREATE
	}
}

LRESULT Window::SubclassedWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* ptr = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	return ptr->ProcessMessage(hwnd, msg, wParam, lParam);
}

bool Window::RegisterWndClass(UINT style, HICON icon, HCURSOR cursor, HBRUSH backgroundBrush, LPCWSTR className) {
	m_ClassName = className;
	
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = RegisteredWindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = NULL;
	wcx.hIcon = icon;
	wcx.hCursor = cursor;
	wcx.hbrBackground = (HBRUSH)backgroundBrush;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = m_ClassName;
	wcx.hIconSm = (HICON)NULL;

	return RegisterClassEx(&wcx);
}