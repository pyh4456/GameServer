#pragma once
#include "JobQueue.h"

#define MAX_NUM_OF_ENEMY 10

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

public:
	bool EnterRoom(ObjectRef object, bool randPos = true);
	bool LeaveRoom(ObjectRef object);

	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);
	bool HandleEnterRoom(PlayerRef player);
	void HandleMove(Protocol::C_MOVE pkt);
	void HandleScore(Protocol::C_SCORE pkt);

	void SpawnEnemy();
	void SetCoordinates(int64 x, int64 y);

public:
	void UpdateTick();

	RoomRef GetRoomRef();

private:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, ObjectRef> _objects;
	uint64 _numOfEnemy;
	int64 _originCoordinatesX;
	int64 _originCoordinatesY;
};

extern class vector<RoomRef> Rooms;