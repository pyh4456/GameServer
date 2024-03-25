#pragma once
class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayerLocked(PlayerRef player);

private:
	bool EnterPlayer(PlayerRef player);

	USE_LOCK;

private:
	unordered_map<uint64, PlayerRef> _players;
};

extern RoomRef GRoom;