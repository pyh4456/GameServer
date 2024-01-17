#pragma once
#include "JobSerializer.h"

class Room : public JobSerializer
{
public:
	// 싱글 스레드 환경 처리
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

public:
	// 멀티 스레드 환경은 일감으로 처리 
	virtual void FlushJob() override;

private:
	map<uint64, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;

