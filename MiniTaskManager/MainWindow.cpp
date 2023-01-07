#include "MainWindow.h"

MainWindow::MainWindow() {
	HINSTANCE hInstance = GetModuleHandle(NULL);

	RegisterWndClass(hInstance);
	CreateWnd(hInstance);

}

bool MainWindow::RegisterWndClass(HINSTANCE hInstance) {
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW; 
	wcx.lpfnWndProc = MainWindow::WindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcx.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 
	wcx.lpszMenuName = NULL; 
	wcx.lpszClassName = m_ClassName; 
	wcx.hIconSm = (HICON) LoadImage(hInstance,
		MAKEINTRESOURCE(5),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	return RegisterClassEx(&wcx);
}

bool MainWindow::CreateWnd(HINSTANCE hInstance) {
	m_WindowHandle = CreateWindow(
		m_ClassName, 
		m_WindowName, 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL
	);

	return m_WindowHandle;
}

LRESULT MainWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			break;
		default:
			return DefWindowProc(hwnd , msg , wParam , lParam);
	}
}

void MainWindow::ProcessMessages() {
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
