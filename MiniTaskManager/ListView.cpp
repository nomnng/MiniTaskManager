#include "ListView.h"

ListView::ListView(HWND parent, int x, int y, int width, int height)
	: Window(parent, x, y, width, height, TEXT("")) {

	CreateWnd(WC_LISTVIEW, WS_CHILD | WS_VISIBLE | LVS_REPORT, parent, true);

	ListView_SetExtendedListViewStyle(m_WindowHandle, LVS_EX_FULLROWSELECT);
}

LRESULT ListView::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		default:
			return CallWindowProc(m_OriginalWindowProc, hwnd, msg, wParam, lParam);
	}
}

void ListView::AddColumn(LPTSTR name, int width, int minWidth, int columnIndex) {
	LVCOLUMN columnInfo;
	columnInfo.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_MINWIDTH;
	columnInfo.fmt = LVCFMT_LEFT;
	columnInfo.cx = width;
	columnInfo.pszText = name;
	columnInfo.iSubItem = columnIndex;
	columnInfo.cxMin = minWidth;

	SendMessage(m_WindowHandle, LVM_INSERTCOLUMN, columnIndex, (LPARAM)&columnInfo);
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

LPARAM ListView::CreateItemAtIndex(LPTSTR item, int index) {
	LVITEM itemInfo;
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = index;
	itemInfo.iSubItem = 0;
	itemInfo.pszText = item;

	m_ItemCount++;
	return SendMessage(m_WindowHandle, LVM_INSERTITEM, 0, (LPARAM)&itemInfo);
}

LPARAM ListView::CreateItem(LPTSTR item) {
	LVITEM itemInfo;
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = m_ItemCount++;
	itemInfo.iSubItem = 0;
	itemInfo.pszText = item;

	return SendMessage(m_WindowHandle, LVM_INSERTITEM, 0, (LPARAM)&itemInfo);
}

void ListView::DeleteAllItems() {
	SendMessage(m_WindowHandle, LVM_DELETEALLITEMS, 0, 0);
	m_ItemCount = 0;
}

void ListView::DeleteItem(int index) {
	SendMessage(m_WindowHandle, LVM_DELETEITEM, index, 0);
}

void ListView::ChangeItem(LPTSTR data, int itemIndex, int subItemIndex) {
	LVITEM itemInfo;
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = itemIndex;
	itemInfo.iSubItem = subItemIndex;
	itemInfo.pszText = data;
	SendMessage(m_WindowHandle, LVM_SETITEM, 0, (LPARAM)&itemInfo);
}

void ListView::UpdateColumnWidth() {
	ListView_SetColumnWidth(m_WindowHandle, m_ColumnCount - 1, LVSCW_AUTOSIZE_USEHEADER);
}

