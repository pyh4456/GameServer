#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "ClientPacketHandler.h"
#include "DummySession.h"
#include "DummyManager.h"

char sendData[] = "Hello World";

int main()
{
	int64 numOfDummy = 0;

	cout << "Enter Number : ";
	cin >> numOfDummy;

	ClientPacketHandler::Init();

	this_thread::sleep_for(1s);

	ClientServiceRef service = make_shared<ClientService>(
		NetAddress(L"127.0.0.1", nullptr, 7777, true),
		make_shared<IocpCore>(),
		[=]() { return make_shared<DummySession>(); }, numOfDummy);

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

	GThreadManager->Launch([=]()
	{
		while (true)
		{
			GDummyManager.Update();

			this_thread::sleep_for(1s);
		}
	});

	string s;

	while (true) {
		cout << "Enter 'quit' when to quit game : ";
		cin >> s;

		if (s == "quit")
		{
			GDummyManager.Quit();
			break;
		}
	}
	

	GThreadManager->Join();

}