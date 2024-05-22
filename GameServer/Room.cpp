#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"
#include "ObjectUtils.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
	_numOfEnemy = 0;
}

Room::~Room()
{
}

bool Room::EnterRoom(ObjectRef object, bool randPos)
{
	bool success = AddObject(object);

	if (randPos) {
		object->posInfo->set_x(Utils::GetRandom(-1500.f, 1500.f));
		object->posInfo->set_y(Utils::GetRandom(-1500.f, 1500.f));
		object->posInfo->set_z(Utils::GetRandom(0.f, 200.f));
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	}

	// 입장 사실을 신입 플레이어에게 알린다.
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다.
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	//기존에 입장한 오브젝트 목록을 플레이어에게 전송한다.
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _objects)
		{
			//if (item.second->IsPlayer() == false)
			//	continue;

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
			objectInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;


	const uint64 objectId = object->objectInfo->object_id();
	bool success = RemoveObject(objectId);

	// 퇴장 사실을 해당 플레이어에게 알린다.
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		//Protocol::S_LEAVE_GAME leaveGamePkt;

		//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		//if (auto session = player->session.lock())
		//	session->Send(sendBuffer);
	}

	// 퇴장 사실을 다른 플레이어에게 알린다.
	{
		Protocol::S_DESPAWN despqwnPkt;

		Protocol::ObjectInfo* objectInfo = despqwnPkt.add_objects();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despqwnPkt);
		Broadcast(sendBuffer, objectId);
	}

	return success;
}

bool Room::HandleEnterPlayer(PlayerRef player)
{
	return EnterRoom(player, true);
}

bool Room::HandleLeavePlayer(PlayerRef player)
{
	return LeaveRoom(player);
}

void Room::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandleScore(Protocol::C_SCORE pkt)
{
	const uint64 playerId = pkt.player_id();
	if (_objects.find(playerId) == _objects.end())
		return;

	const uint64 monsterId = pkt.monster_id();
	if (_objects.find(monsterId) == _objects.end())
		return;

	{
		MonsterRef monster = dynamic_pointer_cast<Monster>(_objects[monsterId]);
		LeaveRoom(monster);
		Protocol::S_SCORE scorePkt;
		{
			scorePkt.set_score_decision(true);
			scorePkt.set_point(monster->score);
		}
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(scorePkt);
		PlayerRef player = dynamic_pointer_cast<Player>(_objects[playerId]);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}
}

void Room::SpawnEnemy()
{
	while (_numOfEnemy < MAX_NUM_OF_ENEMY)
	{
		MonsterRef enemy = ObjectUtils::CreateMonster();
		enemy->posInfo->set_x(Utils::GetRandom(2000, 4000));
		enemy->posInfo->set_y(Utils::GetRandom(2000, 4000));
		enemy->posInfo->set_z(Utils::GetRandom(0.f, 200.f));
		enemy->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
		enemy->score = 100;

		EnterRoom(enemy, false);

		_numOfEnemy++;
	}
}

void Room::UpdateTick()
{
	SpawnEnemy();
	cout << "Spawn Enemy\n";

	DoTimer(10000, &Room::UpdateTick);
}

RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddObject(ObjectRef object)
{
	// 이미 해당 오브젝트가 있다.
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	if (object->objectInfo->object_type() == Protocol::OBJECT_TYPE_ENEMY)
		_numOfEnemy--;

	object->room.store(weak_ptr<Room>());

	_objects.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _objects)
	{
		if (item.second->IsPlayer() == false)
			continue;

		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			continue;

		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
		
	}
}
