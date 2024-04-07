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

	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 2; i++)
	{
		Protocol::ObjectInfo* player = loginPkt.add_players();
		Protocol::PosInfo* posInfo = player->mutable_pos_info();
		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
		player->set_player_type(Protocol::PlayerType(i + 1));
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
	switch (pkt.playerindex())
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
	//room->HandleLeavePlayer(player);

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
