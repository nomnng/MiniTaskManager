#include "MainWindow.h"

MainWindow::MainWindow(LPCWSTR windowName, int width, int height)
	: Window(NULL, CW_USEDEFAULT, CW_USEDEFAULT, width, height, windowName) {

	HBRUSH backgroundBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	RegisterWndClass(CS_HREDRAW | CS_VREDRAW, NULL, NULL, backgroundBrush, MAIN_WINDOW_CLASS_NAME);
	CreateWnd(m_ClassName, WS_OVERLAPPEDWINDOW, NULL, false);

	SetTimer(m_WindowHandle, TIMER_ID1, 1000, (TIMERPROC)NULL);

	m_ProcessListView = new ListView(m_WindowHandle, 0, 0, m_Width, m_Height - 200);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("PID"), 50, 50);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("Process name"), m_Width / 2, 200);
	
	InitProcessList();

	ShowWindow(m_WindowHandle, SW_SHOW);
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
			return ProcessNotification(wParam, lParam);
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
		m_ProcessListView->SetRect(0, 50, m_Width, m_Height - 100);
		ListView_SetColumnWidth(m_ProcessListView->GetHWND(), 1, LVSCW_AUTOSIZE_USEHEADER);
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

	vector<DWORD> newProcessIDs = ProcessEnumerator::GetProcessIDs();

	vector<DWORD> createdProcesses = Utils::vectorDifference(newProcessIDs, m_ProcessIDs);
	vector<DWORD> endedProcesses = Utils::vectorDifference(m_ProcessIDs, newProcessIDs);

	for (int i = 0; i < endedProcesses.size(); i++) {
		for (int j = 0; j < m_ProcessIDs.size(); j++) {
			if (m_ProcessIDs[j] == endedProcesses[i]) {
				m_ProcessListView->DeleteItem(j); // removing ended processes from listview
			}
		}
	}

	for (int i = 0; i < createdProcesses.size(); i++) {
		for (int j = 0; j < newProcessIDs.size(); j++) {
			if (newProcessIDs[j] == createdProcesses[i]) {
				wstring pidStr = to_wstring(createdProcesses[i]);
				m_ProcessListView->CreateItemAtIndex((LPTSTR)pidStr.c_str(), j); // adding created processes to listview

				wstring pName = ProcessEnumerator::GetProcessName(createdProcesses[i]);
				m_ProcessListView->ChangeItem((LPTSTR)pName.c_str(), j, COLUMN_NAME_INDEX);
			}
		}

	}

	m_ProcessIDs = newProcessIDs;
}

void MainWindow::InitProcessList() {
	if (!m_ProcessListView)
		return;

	m_ProcessIDs = ProcessEnumerator::GetProcessIDs();

	for (int i = 0; i < m_ProcessIDs.size(); i++) {
		wstring pidStr = to_wstring(m_ProcessIDs[i]);
		m_ProcessListView->CreateItem((LPTSTR)pidStr.c_str());

		wstring pName = ProcessEnumerator::GetProcessName(m_ProcessIDs[i]);

		m_ProcessListView->ChangeItem((LPTSTR)pName.c_str(), i, COLUMN_NAME_INDEX);
	}
}

LRESULT MainWindow::ProcessNotification(WPARAM wParam, LPARAM lParam) {
	LPNMLVCUSTOMDRAW  customDrawInfo = (LPNMLVCUSTOMDRAW)lParam;
	int colorFlag = 0;
	
	switch (customDrawInfo->nmcd.hdr.code) {
		case NM_CUSTOMDRAW:
			switch (customDrawInfo->nmcd.dwDrawStage) {
				case CDDS_PREPAINT:
					return CDRF_NOTIFYITEMDRAW;

				case CDDS_ITEMPREPAINT:
					colorFlag = customDrawInfo->nmcd.dwItemSpec % 2;
					customDrawInfo->clrText = RGB(0, 0, 0);
					customDrawInfo->clrTextBk = RGB(130 - (colorFlag * 50), 180 - (colorFlag * 50), 250);

					return CDRF_NEWFONT;
			}
		default:
			return 0;
	}
}



void MainWindow::MessageLoop() {
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
