#pragma once
#include "Job.h"

class Room
{
public:
	// 싱글 스레드 환경 처리
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

public:
	// 멀티 스레드 환경은 일감으로 처리 
	void PushJob(JobRef job) { _jobs.Push(job); }
	void FlushJob();

	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* memFunc)(Args...), Args... args)
	{
		auto job = MakeShared<MemberJob<T, Ret, Args...>>(static_cast<T*>(this), memFunc, args...);
		_jobs.Push(job);
	}

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;

	JobQueue _jobs;
};

extern Room GRoom;

