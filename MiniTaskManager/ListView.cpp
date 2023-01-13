#include "ListView.h"

ListView::ListView(HWND parent, int x, int y, int width, int height)
	: Window(parent, x, y, width, height, TEXT("")) {

	CreateWnd(WC_LISTVIEW, WS_CHILD | WS_VISIBLE | LVS_REPORT, parent, true);

}

LRESULT ListView::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		default:
			return CallWindowProc(m_OriginalWindowProc, hwnd, msg, wParam, lParam);
	}
}

void ListView::AddColumn(LPTSTR name, int width, int minWidth) {
	LVCOLUMN columnInfo;
	columnInfo.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_MINWIDTH;
	columnInfo.fmt = LVCFMT_LEFT;
	columnInfo.cx = width;
	columnInfo.pszText = name;
	columnInfo.iSubItem = m_ColumnCount;
	columnInfo.cxMin = minWidth;

	SendMessage(m_WindowHandle, LVM_INSERTCOLUMN, m_ColumnCount, (LPARAM)&columnInfo);
	m_ColumnCount++;
}

void ListView::AddItem(vector<LPTSTR> item) {
	LVITEM itemInfo;
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = m_ItemCount;
	itemInfo.iSubItem = 0;
	itemInfo.pszText = item[0];

	SendMessage(m_WindowHandle, LVM_INSERTITEM, 0, (LPARAM)&itemInfo);

	for (int i = 1; i < item.size() && i < m_ColumnCount; i++) {
		itemInfo.iSubItem = i;
		itemInfo.pszText = item[i];
		SendMessage(m_WindowHandle, LVM_SETITEM, 0, (LPARAM)&itemInfo);	
	}

	m_ItemCount++;
}

void ListView::DeleteAllItems() {
	SendMessage(m_WindowHandle, LVM_DELETEALLITEMS, 0, 0);
}

