#include "pch.h"
#include "DummySession.h"

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
	cout << "OnSend Len = " << len << endl;
}

void DummySession::OnDisconnected()
{
	cout << "Disconnected" << endl;
}
