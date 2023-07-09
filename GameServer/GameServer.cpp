#include "pch.h"
#include "CorePch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>	

class SpinLock {
public:
	void lock() {
		//CAS (Compare-And-Swap) 코드를 atomic 하게 실행하도록 컴파일
		bool expected = false;
		bool desired = true;

		while(_locked.compare_exchange_strong(expected, desired) == false) {
			expected = false;
		}
	}

	void unlock() {
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false; // true: locked, false : unlocked
};

int sum;
SpinLock spinLock;

void Add() {
	for (int i = 0; i < 100000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Add);

	t1.join();
	t2.join();

	cout << sum << endl;
}

/*
class SpinLock {
public:
	//critical section에 접근하고 lock을 건다.
	void lock() {
		while (_locked) {
		}
		_locked = true;
	}

	//critical section에서 빠져나오면서 lock을 푼다.
	void unlock() {
		_locked = false;
	}

private:
	bool _locked = false; // true: locked, false : unlocked
};

int sum;
SpinLock spinLock;

void Add() {
	for (int i = 0; i < 100000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Add);

	t1.join();
	t2.join();

	//Spin Lock이 제대로 작동하면 sum이 200000이 된다.
	cout << sum << endl;
}*/