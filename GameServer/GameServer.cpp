#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"
#include "functional"
#include "Player.h"

enum
{
	// TODO : Job이 몰리는 정도에 따라 값을 변동해야 함
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service) {
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		//네트워크 입출력 처리
		service->GetIocpCore()->Dispatch(10);

		//글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}


int main()
{

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
		{
			DoWorkerJob(service);
		});
	}	

	//Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}