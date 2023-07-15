#pragma once
#include"Types.h"

/*----------------
	RW SpinLock
-----------------*/

/*----------------
[wwwwwwww][wwwwwwww][rrrrrrrr][rrrrrrrr]

w : WriteFlag (Exclusive Lock Owner ThreadId)
r : ReadFlag (Shared Lock Count);

w 중에 r을 잡아도 된다.
r 중에 w을 잡으면 안된다.
------------------*/

class Lock
{
	enum : uint32 {
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*-----------------
	LockGuards
-------------------*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnlock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }
private:
	Lock& _lock;
};