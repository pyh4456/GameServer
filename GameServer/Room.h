#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);
	void HandleMove(Protocol::C_MOVE pkt);

	void Update() {};

	RoomRef GetRoomRef();

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, PlayerRef> _players;
};

extern RoomRef GRoom;