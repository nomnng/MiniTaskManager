#include "ProcessEnumerator.h"

vector<wstring> ProcessEnumerator::GetProcessList() {
	vector<DWORD> pids = GetProcessIDs();
	vector<wstring> processNameList;
	for (int i = 0; i < pids.size(); i++) {
		HANDLE pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pids[i]);
		if (pHandle != NULL) {
			TCHAR processName[MAX_PATH];
			int nameLen = GetProcessImageFileName(pHandle, processName, MAX_PATH);
			if (nameLen > 0) {
				wstring nameStr = wstring(processName, nameLen);
				int slashPos = nameStr.find_last_of(TEXT("\\"));
				processNameList.push_back(nameStr.substr(slashPos + 1));
			}
		}
	}

	return processNameList;
}

vector<DWORD> ProcessEnumerator::GetProcessIDs() {
	DWORD pids[MAX_PROCESS_HANDLES];
	DWORD returnedSize;
	EnumProcesses(pids, sizeof(pids), &returnedSize);

	int pCount = returnedSize / sizeof(DWORD);

	vector<DWORD> processList;
	for (int i = 0; i < pCount; i++) {
		processList.push_back(pids[i]);
	}

	return processList;
}
