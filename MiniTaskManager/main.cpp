#include <iostream>
#include <Windows.h>

#include "MainWindow.h"
#include "ProcessEnumerator.h"

using namespace std;

int main() {
	ProcessEnumerator::GetProcessList();

	MainWindow wnd(TEXT("MiniTaskManager") , 800, 600);

	wnd.MessageLoop();

}