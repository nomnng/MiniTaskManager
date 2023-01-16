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
		if (pids[i] == 4) // skip SYSTEM process
			continue;

		HANDLE pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pids[i]);
		if (pHandle != NULL) {		
			processList.push_back(pids[i]);
			CloseHandle(pHandle);
		}
	}

	return processList;
}

wstring ProcessEnumerator::GetProcessName(DWORD pid) {
	HANDLE pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (pHandle != NULL) {
		TCHAR processName[MAX_PATH];
		int nameLen = GetProcessImageFileName(pHandle, processName, MAX_PATH);
		CloseHandle(pHandle);
		if (nameLen > 0) {
			wstring nameStr = wstring(processName, nameLen);
			int slashPos = nameStr.find_last_of(TEXT("\\"));
			return wstring(nameStr.substr(slashPos + 1));
		}
	}

	return wstring();
}

ULONG64 ProcessEnumerator::GetUsedCPUTime(DWORD pid) {
	HANDLE pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (pHandle != NULL) {
		FILETIME creationTime, exitTime, kernelTime, userTime;
		GetProcessTimes(pHandle, &creationTime, &exitTime, &kernelTime, &userTime);

		ULONG64 result = (userTime.dwHighDateTime << 16) + userTime.dwLowDateTime;
		result += (kernelTime.dwHighDateTime << 16) + kernelTime.dwLowDateTime;
		return result;
	}

	return 0;
}
