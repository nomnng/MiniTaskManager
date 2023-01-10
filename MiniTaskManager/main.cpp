#include <iostream>
#include <Windows.h>

#include "MainWindow.h"

using namespace std;

int main() {
	MainWindow wnd(L"MiniTaskManager" , 600, 400);
	wnd.MessageLoop();

}