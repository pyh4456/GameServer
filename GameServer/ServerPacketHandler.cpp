#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "ObjectUtils.h"
#include "Room.h"
#include "Player.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// TODO : DB에서 Account 정보를 긁어온다.

	cout << "Log in ID : " << pkt.id() << endl;
	cout << "Log in Password : " << pkt.password() << endl;

	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ObjectInfo* player = loginPkt.add_players();
		Protocol::PosInfo* posInfo = player->mutable_pos_info();
		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));

		Protocol::CharacterInfo* character = player->mutable_player_info();
		character->set_score(100 * (1 + i));
		if (i == 0)
		{
			character->set_name(pkt.id());
			player->set_player_type(Protocol::PLAYER_TYPE_YOSHIKA);
		}
		else if (i == 1)
		{
			character->set_name(pkt.id());
			player->set_player_type(Protocol::PLAYER_TYPE_LYNETTE);
		}
		else if (i == 2)
		{
			character->set_name(pkt.id());
			player->set_player_type(Protocol::PLAYER_TYPE_SANYA);
		}
	}

	loginPkt.set_success(true);	
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPkt);	\
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// 플레이어 생성
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	// TEMP : 플레이어가 선택한 index에 맞는 캐릭터 정보 불러오기 
	switch (pkt.selectedcharacter().player_type())
	{
	case 1:
		player->objectInfo->set_player_type(Protocol::PLAYER_TYPE_YOSHIKA);
		break;
	case 2:
		player->objectInfo->set_player_type(Protocol::PLAYER_TYPE_LYNETTE);
		break;
	case 3:
		player->objectInfo->set_player_type(Protocol::PLAYER_TYPE_SANYA);
		break;
	}

	player->objectInfo->mutable_player_info()->set_name(pkt.selectedcharacter().player_info().name());
	player->objectInfo->mutable_player_info()->set_score(pkt.selectedcharacter().player_info().score());

	// 방 입장
	GRoom->DoAsync(&Room::HandleEnterPlayer, player);
	//GRoom->HandleEnterPlayer(player);

	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::HandleLeavePlayer, player);

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::HandleMove, pkt);
	//room->HandleMove(pkt);
	
	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	auto Msg = pkt.msg();

	cout << Msg << endl;

	return true;
}

bool Handle_C_SHOOT(PacketSessionRef& session, Protocol::C_SHOOT& pkt)
{
	return false;
}

bool Handle_C_REMOVE_BULLET(PacketSessionRef& session, Protocol::C_REMOVE_BULLET& pkt)
{
	return false;
}

bool Handle_C_SCORE(PacketSessionRef& session, Protocol::C_SCORE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	GRoom->DoAsync(&Room::HandleScore, pkt);

	return false;
}
