#include "MainWindow.h"

MainWindow::MainWindow(LPCWSTR windowName, int width, int height)
	: Window(NULL, CW_USEDEFAULT, CW_USEDEFAULT, width, height, windowName) {

	HBRUSH backgroundBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	RegisterWndClass(CS_HREDRAW | CS_VREDRAW, NULL, NULL, backgroundBrush, MAIN_WINDOW_CLASS_NAME);
	CreateWnd(m_ClassName, WS_OVERLAPPEDWINDOW, NULL, false);

	SetTimer(m_WindowHandle, TIMER_ID1, TIMER_INTERVAL_MS, (TIMERPROC)NULL);

	m_ProcessListView = new ListView(m_WindowHandle, 0, 0, m_Width, m_Height);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("PID"), 50, 50, COLUMN_PID_INDEX);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("Process name"), m_Width / 2, 200, COLUMN_NAME_INDEX);
	m_ProcessListView->AddColumn((LPTSTR)TEXT("CPU"), m_Width / 2, 100, COLUMN_CPU_INDEX);

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
		m_ProcessListView->SetRect(0, 0, m_Width, m_Height);
		m_ProcessListView->UpdateColumnWidth();
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
				m_ProcessUsedCPU.erase(m_ProcessUsedCPU.begin() + j);
				m_ProcessIDs.erase(m_ProcessIDs.begin() + j);
			}
		}
	}

	for (int i = 0; i < createdProcesses.size(); i++) {
		for (int j = 0; j < newProcessIDs.size(); j++) {
			if (newProcessIDs[j] == createdProcesses[i]) {
				DWORD pid = createdProcesses[i];
				wstring pidStr = to_wstring(pid);
				m_ProcessListView->CreateItemAtIndex((LPTSTR)pidStr.c_str(), j); // adding created processes to listview

				wstring pName = ProcessEnumerator::GetProcessName(pid);
				m_ProcessListView->ChangeItem((LPTSTR)pName.c_str(), j, COLUMN_NAME_INDEX);

				m_ProcessListView->ChangeItem((LPTSTR)TEXT("0"), j, COLUMN_CPU_INDEX);

				m_ProcessUsedCPU.insert(m_ProcessUsedCPU.begin() + j, ProcessEnumerator::GetUsedCPUTime(pid));
				m_ProcessIDs.insert(m_ProcessIDs.begin() + j, pid);
			}
		}
	}

	for (int i = 0; i < newProcessIDs.size(); i++) {
		ULONG64 newTime = ProcessEnumerator::GetUsedCPUTime(newProcessIDs[i]);
		if (m_ProcessUsedCPU[i] != newTime) {
			ULONG64 usage = (newTime - m_ProcessUsedCPU[i]) / (TIMER_INTERVAL_MS * 1000);
			
			wstring usageStr = to_wstring(usage);

			m_ProcessListView->ChangeItem((LPTSTR)usageStr.c_str(), i, COLUMN_CPU_INDEX);

			m_ProcessUsedCPU[i] = newTime;
			
		}
	}
}

void MainWindow::InitProcessList() {
	if (!m_ProcessListView)
		return;

	m_ProcessIDs = ProcessEnumerator::GetProcessIDs();

	for (int i = 0; i < m_ProcessIDs.size(); i++) {
		DWORD pid = m_ProcessIDs[i];

		wstring pidStr = to_wstring(pid);
		m_ProcessListView->CreateItem((LPTSTR)pidStr.c_str());

		wstring pName = ProcessEnumerator::GetProcessName(m_ProcessIDs[i]);
		m_ProcessListView->ChangeItem((LPTSTR)pName.c_str(), i, COLUMN_NAME_INDEX);

		m_ProcessListView->ChangeItem((LPTSTR)TEXT("0"), i, COLUMN_CPU_INDEX);
		m_ProcessUsedCPU.push_back(ProcessEnumerator::GetUsedCPUTime(pid));
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
