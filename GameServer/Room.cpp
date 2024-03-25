#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterPlayer(player);

	player->playerInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_z(Utils::GetRandom(0.f, 500.f));
	player->playerInfo->set_yaw(Utils::GetRandom(0.f, 100.f));

	// 입장 사실을 신입 플레이어에게 알린다.
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterGamePkt.set_allocated_player(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}
	// 입장 사실을 다른 플레이어에게 알린다.
	// TODO

	return false;
}

bool Room::EnterPlayer(PlayerRef player)
{
	// 이미 플레이어가 있다.
	if (_players.find(player->playerInfo->object_id()) != _players.end());
		return false;

	_players.insert(make_pair(player->playerInfo->object_id(), player));

	player->room.store(shared_from_this());

	return true;
}
