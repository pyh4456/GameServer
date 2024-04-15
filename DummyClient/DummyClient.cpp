#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "ClientPacketHandler.h"
#include "DummySession.h"

char sendData[] = "Hello World";

int main()
{
	ClientPacketHandler::Init();

	this_thread::sleep_for(1s);

	ClientServiceRef service = make_shared<ClientService>(
		NetAddress(L"192.168.35.92", nullptr, 7777, true),
		make_shared<IocpCore>(),
		[=]() { return make_shared<DummySession>(); }, // TODO : SessionManager 등
		1);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	while (true)
	{
		//service->Broadcast(sendBuffer);
		this_thread::sleep_for(1s);
	}

	GThreadManager->Join();

}