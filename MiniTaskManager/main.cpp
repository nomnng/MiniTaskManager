#include <iostream>
#include <Windows.h>

#include "MainWindow.h"
#include "ProcessEnumerator.h"

using namespace std;

int main() {
	ProcessEnumerator::GetProcessList();

	MainWindow wnd(L"MiniTaskManager" , 600, 400);
	wnd.MessageLoop();

}