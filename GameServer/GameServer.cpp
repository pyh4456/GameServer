#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>	
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

class TestLock {
	USE_LOCK;

public:
	int32 TestRead() {
		READ_LOCK;

		if (_queue.empty())
			return - 1;
	}
	void TestPush() {
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop() {
		WRITE_LOCK;

		if (_queue.empty() == false)
			_queue.pop();
	}

private:
	queue<int32> _queue;
};

TestLock testLock;

void ThreadWrite() {
	while (true) {
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead() {
	while (true) {
		int32 value = testLock.TestRead();
		cout << value << endl; 
		this_thread::sleep_for(1ms);
	}
}

int main() {
	for (int32 i = 0; i < 200; i++) {
		GThreadManager->Launch(ThreadWrite);
	}


	for (int32 i = 0; i < 300; i++) {
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();
}
	