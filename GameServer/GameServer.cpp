#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

class Player {
public:
	Player() {}
	virtual ~Player() {	}
};

class Knight : public Player {
public:
	Knight() {
		cout << "Knight()" << endl;
	}
	~Knight() {
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 10;

};



int main() {

	for (int32 i = 0; i < 5; i++) {
		GThreadManager->Launch([]()
			{
				while (true) {
					Vector<Knight> v(10);

					Map<int32, Knight> m;
					m[100] = Knight();

					this_thread::sleep_for(10ms);
				}
			}
		);
	}

	GThreadManager->Join();
}