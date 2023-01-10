#include "Window.h"

Window::Window(HWND parent, int x, int y, int width, int height, LPCWSTR windowName)
	: m_Parent(parent), m_Position{x, y}, m_Width(width), m_Height(height), m_WindowName(windowName), m_OriginalWindowProc(0), m_WindowHandle(0) {}

bool Window::CreateWnd(LPCWSTR className, DWORD style, HWND parent) {
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
		(LPVOID)NULL
	);

	return m_WindowHandle != NULL;
}

void Window::SetWindowProc(bool subclassed) {
	SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, (LONG)this);
	if (subclassed) {
		m_OriginalWindowProc = (WNDPROC)SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, (LONG)SubclassedWindowProc);
	} else {
		SetWindowLongPtr(m_WindowHandle, GWLP_WNDPROC, (LONG)WindowProc);
	}
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* ptr = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (ptr != NULL)
		return ptr->ProcessMessage(hwnd, msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::SubclassedWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* ptr = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	return CallWindowProc(ptr->m_OriginalWindowProc, hwnd, msg, wParam, lParam);
}

LRESULT Window::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

