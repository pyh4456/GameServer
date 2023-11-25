#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"

class GameSession : public Session
{
public:
	~GameSession()
	{
		cout << "Client Disconnected" << endl;
	}

	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		// Echo
		cout << "OnRecv Len = " << len << endl;
		cout << "OnRecv Data = " << buffer << endl;
		Send(buffer, len);
		return len;
	}
	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};


int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		// TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}				
			});
	}	

	GThreadManager->Join();
}