#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include <tchar.h>
#include "Job.h"
#include "Protocol.pb.h"
#include "Room.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	ServerPacketHandler::Init();

	// 서비스 초기화
	ServerServiceRef service = make_shared<ServerService>(
		//NetAddress(L"192.168.35.92", nullptr, 7777, true),
		NetAddress(L"127.0.0.1", nullptr, 7777, true),
		make_shared<IocpCore>(),
		[=]() { return make_shared<GameSession>(); },
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}


	// 맵 초기화
	for (int i = 0; i < 9; i++)
	{	
		Rooms[i] = make_shared<Room>();
	}

	Rooms[0]->SetCoordinates(0, 0);
	Rooms[1]->SetCoordinates(6666, 6666);
	Rooms[2]->SetCoordinates(0, 6666);
	Rooms[3]->SetCoordinates(-6666, 6666);
	Rooms[4]->SetCoordinates(6666, 0);
	Rooms[5]->SetCoordinates(-6666, 0);
	Rooms[6]->SetCoordinates(6666, -6666);
	Rooms[7]->SetCoordinates(0, -6666);
	Rooms[8]->SetCoordinates(-6666, -6666);

	for (int i = 0; i < 9; i++)
	{
		Rooms[i]->DoAsync(&Room::UpdateTick);
	}
	
	GThreadManager->Join();
}