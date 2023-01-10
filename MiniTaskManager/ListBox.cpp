#include "ListBox.h"

ListBox::ListBox(HWND parent, int x, int y, int width, int height)
	: Window(parent, x, y, width, height, L"") {

	CreateWnd(WC_LISTBOX, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY, parent);
	SetWindowProc(true);

}

void ListBox::AddItem(LPWSTR str) {
	SendMessage(m_WindowHandle, LB_ADDSTRING, 0, (LPARAM)str);
}

void ListBox::AddItems(vector<LPWSTR> items) {
	for (int i = 0; i < items.size(); i++) {
		AddItem(items[i]);
	}
}
