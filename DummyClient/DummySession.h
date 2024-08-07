#pragma once
#include "ClientPacketHandler.h"

class DummySession : public PacketSession
{
public:
	~DummySession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override;

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;

	virtual void OnSend(int32 len) override;

	virtual void OnDisconnected() override;

	void Quit();

public:
	void SetDummyInfo(Protocol::ObjectInfo info);
	void RandomAction();

public:
	Protocol::ObjectInfo _dummyInfo;
};