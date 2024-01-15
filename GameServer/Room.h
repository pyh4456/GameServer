#pragma once
#include "Job.h"

class Room
{
public:
	// �̱� ������ ȯ�� ó��
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

public:
	// ��Ƽ ������ ȯ���� �ϰ����� ó�� 
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

