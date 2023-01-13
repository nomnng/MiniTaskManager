#include "MainWindow.h"

MainWindow::MainWindow(LPCWSTR windowName, int width, int height)
	: Window(NULL, CW_USEDEFAULT, CW_USEDEFAULT, width, height, windowName) {

	HBRUSH backgroundBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	RegisterWndClass(CS_HREDRAW | CS_VREDRAW, NULL, NULL, backgroundBrush, MAIN_WINDOW_CLASS_NAME);
	CreateWnd(m_ClassName, WS_OVERLAPPEDWINDOW, NULL, false);

	SetTimer(m_WindowHandle, TIMER_ID1, 2000, (TIMERPROC)NULL);

	m_ProcessListView = new ListView(m_WindowHandle, 0, 0, m_Width, m_Height);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("Process name"), m_Width - 40, 200);

	ShowWindow(m_WindowHandle, SW_SHOW);

	UpdateProcessList();
}

LRESULT MainWindow::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_KEYDOWN:
			cout << "KeyDown:" << wParam << endl;
			return 0;
		case WM_CLOSE:
			ExitProcess(0);
			return 0;
		case WM_SIZE:
			OnResize(wParam , lParam);
			return 0;
		case WM_COMMAND:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				cout << "SELCHANGE" << endl;
			return 0;
		case WM_NOTIFY:
			return 0;
		case WM_TIMER:
			OnTimer(wParam);
			return 0;
		default:
			return DefWindowProc(hwnd , msg , wParam , lParam);
	}
}

void MainWindow::OnResize(WPARAM wParam, LPARAM lParam) {
	DWORD resizeType = wParam;
	m_Width = LOWORD(lParam);
	m_Height = HIWORD(lParam);

	if (m_ProcessListView) {
		HWND listView = m_ProcessListView->GetHWND();
		m_ProcessListView->SetRect(0, 50, m_Width, m_Height - 100);
	}
}

void MainWindow::OnTimer(WPARAM wParam) {
	UINT timerID = wParam;
	switch (timerID) {
		case TIMER_ID1:
			UpdateProcessList();
			break;
	}
}

void MainWindow::UpdateProcessList() {
	if (!m_ProcessListView)
		return;

	m_ProcessListView->DeleteAllItems();
	vector<wstring> processNames = ProcessEnumerator::GetProcessList();
	for (int i = 0; i < processNames.size(); i++) {
		vector<LPTSTR> item = vector<LPTSTR>();
		item.push_back((LPTSTR)processNames[i].c_str());
		m_ProcessListView->AddItem(item);
	}
}



void MainWindow::MessageLoop() {
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
