#include "pch.h"
#include "DummySession.h"
#include "Utils.h"

void DummySession::OnConnected()
{
	cout << "OnConnected" << endl;

	Protocol::C_ENTER_GAME pkt;

	Protocol::ObjectInfo *info = new Protocol::ObjectInfo;
	pkt.set_allocated_selectedcharacter(info);
	info->set_player_type(Protocol::PLAYER_TYPE_YOSHIKA);
	info->mutable_player_info()->set_name("Dummy");
	info->mutable_player_info()->set_score(0);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	Send(sendBuffer);

	GDummyManager.Add(static_pointer_cast<DummySession>(shared_from_this()));
}

void DummySession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void DummySession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}

void DummySession::OnDisconnected()
{
	cout << "Disconnected" << endl;
}

void DummySession::Quit()
{
	Protocol::C_LEAVE_GAME LeavePkt;

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(LeavePkt);

	Send(sendBuffer);
}

void DummySession::SetDummyInfo(Protocol::ObjectInfo info)
{
	_dummyInfo.CopyFrom(info);
}

void DummySession::RandomAction()
{
	int64 action = Utils::GetRandom(0, 7);
	Protocol::PosInfo* posInfo;

	switch (action)
	{
	case 0:		//no move
		break;
	case 1:		//move +X
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_x(posInfo->x() + 40);
		break;
	case 2:		//move -X
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_x(posInfo->x() - 40);
		break;
	case 3:		//move +Y
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_y(posInfo->y() + 40);
		break;
	case 4:		//move -Y
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_y(posInfo->y() - 40);
		break;
	case 5:		//move +Z
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_z(posInfo->z() + 40);
		break;
	case 6:		//move -Z
		posInfo = _dummyInfo.mutable_pos_info();
		posInfo->set_z(posInfo->z() - 40);
		break;
	}
}
