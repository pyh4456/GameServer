#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>	
#include <future>
#include <Windows.h>
#include "ThreadManager.h"


CoreGlobal Core;

void ThreadMain() {
	while (true) {
		cout << "hello thread num :" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main() {
	for (int32 i = 0; i < 5; i++) {
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();
}
	