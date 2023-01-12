#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <string>

using namespace std;

#define MAX_PROCESS_HANDLES 1024

class ProcessEnumerator {
	public:
		static vector<wstring> GetProcessList();
		static vector<DWORD> GetProcessIDs();
};