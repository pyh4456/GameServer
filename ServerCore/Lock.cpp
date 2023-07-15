#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h":

void Lock::WriteLock()
{
	//동일한 스레드가 lock을 소유하고 있다면 무조건 성공.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		_writeCount++;
		return;
	}

	//아무도 소유 및 공유하고 있지 안을 때, 경합해서 소유권을 얻는다.
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++) {
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(expected, desired)) {
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("INVALID_INLOCK_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	//동일한 스레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId) {
		_lockFlag.fetch_add(1);
		return;
	}

	//아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 증가.
	const int64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++) {
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");


}
