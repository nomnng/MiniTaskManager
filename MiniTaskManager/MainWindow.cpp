#include "MainWindow.h"

MainWindow::MainWindow(LPCWSTR windowName, int width, int height)
	: Window(NULL, CW_USEDEFAULT, CW_USEDEFAULT, width, height, windowName) {

	RegisterWndClass();
	CreateWnd(m_ClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, NULL);
	SetWindowProc(false);

	m_ProcessListBox = new ListBox(m_WindowHandle, 0 , 0 , m_Width , m_Height);

	for (int i = 0; i < 125; i++) {
		wstring str = wstring(L"Test") + to_wstring(i);
		m_ProcessListBox->AddItem((LPWSTR) str.c_str());	
	}


}

bool MainWindow::RegisterWndClass() {
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW; 
	wcx.lpfnWndProc = WindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = NULL;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcx.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH); 
	wcx.lpszMenuName = NULL; 
	wcx.lpszClassName = m_ClassName; 
	wcx.hIconSm = (HICON) LoadImage(NULL,
		MAKEINTRESOURCE(1),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	return RegisterClassEx(&wcx);
}

LRESULT MainWindow::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_SIZE:
			OnResize(wParam , lParam);
			break;
		case WM_COMMAND:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				cout << "SELCHANGE" << endl;
			break;
		default:
			return DefWindowProc(hwnd , msg , wParam , lParam);
	}
}

void MainWindow::OnResize(WPARAM wParam, LPARAM lParam) {
	DWORD resizeType = wParam;
	WORD width = HIWORD(lParam);
	WORD height = LOWORD(lParam);
}

void MainWindow::MessageLoop() {
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
