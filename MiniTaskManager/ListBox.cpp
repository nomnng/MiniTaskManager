#include "ListBox.h"

ListBox::ListBox(HWND parent, int x, int y, int width, int height)
	: Window(parent, x, y, width, height, L"") {

	CreateWnd(WC_LISTBOX, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY, parent, true);
	SendMessage(m_WindowHandle, LB_INITSTORAGE, 1000, 1024 * 100);
	
}

void ListBox::AddItem(LPWSTR str) {
	SendMessage(m_WindowHandle, LB_INSERTSTRING, 0, (LPARAM)str);
	m_ItemCounter++;
}

void ListBox::AddItems(vector<LPWSTR> items) {
	for (int i = 0; i < items.size(); i++) {
		AddItem(items[i]);
	}
}

void ListBox::ClearItems() {
	SendMessage(m_WindowHandle, LB_RESETCONTENT, 0, 0);

	m_ItemCounter = 0;
}

void ListBox::ReplaceItem(LPWSTR str, int index) {
	SendMessage(m_WindowHandle, LB_DELETESTRING, index, 0);
	SendMessage(m_WindowHandle, LB_INSERTSTRING, index, (LPARAM)str);
}

LRESULT ListBox::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_MOUSEWHEEL:
			OnScroll(wParam, lParam);
			return 0;
		default:
			return CallWindowProc(m_OriginalWindowProc, hwnd, msg, wParam, lParam);
	}
}

void ListBox::OnScroll(WPARAM wParam, LPARAM lParam) {
	int scrollDistance = ((short) HIWORD(wParam)) / WHEEL_DELTA;
	if (m_ScrollOffset - scrollDistance < m_ItemCounter)
		m_ScrollOffset -= scrollDistance;

	if (m_ScrollOffset < 0)
		m_ScrollOffset = 0;

	SetScrollPosition();
}

void ListBox::SetScrollPosition() {
	SendMessage(m_WindowHandle, LB_SETCURSEL, m_ScrollOffset, 0);
}


