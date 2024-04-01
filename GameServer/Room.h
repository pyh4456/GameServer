#pragma once
class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);
	void HandleMoveLocked(Protocol::C_MOVE& pkt);

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);

	USE_LOCK;

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, PlayerRef> _players;
};

extern RoomRef GRoom;