#pragma once

class GameSession;
class Room;

class Player : public enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

public:
	Protocol::PlayerInfo* playerInfo;
	weak_ptr<GameSession> session;

public:
	atomic<weak_ptr<Room>> room;
};

