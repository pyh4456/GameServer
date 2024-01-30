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
#include "Player.h"
#include "DBConnectionPool.h""
#include "DBBind.h""

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

		//예약된 일감 처리
		ThreadManager::DstributeReservedJobs();

		//글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}


int main()
{
	//
	GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDB;Trusted_Connection=Yes;");

	// Create Table
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY,	\
				[gold] INT NULL,						\
				[name] NVARCHAR(50) NULL,				\
				[createDate] DATETIME NULL				\
			);";

		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		
		DBBind<3, 0>dbBind(*dbConn, L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");
		
		int32 gold = 100;
		dbBind.BindParam(0, gold);
		WCHAR name[100] = L"nokor";
		dbBind.BindParam(1, name);
		TIMESTAMP_STRUCT ts = {};
		ts.year = 2021;
		ts.month = 5;
		ts.day = 29;
		dbBind.BindParam(2, ts);

		ASSERT_CRASH(dbBind.Execute());


		/*dbConn->Unbind();

		//넘길 인자
		int32 gold = 100;
		SQLLEN len = 0;

		WCHAR name[100] = L"nokor";
		SQLLEN nameLen = 0;

		TIMESTAMP_STRUCT ts = {};
		ts.year = 2021;
		ts.month = 5;
		ts.day = 29;
		SQLLEN tsLen = 0;

		//넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));
		ASSERT_CRASH(dbConn->BindParam(2, name, &nameLen));
		ASSERT_CRASH(dbConn->BindParam(3, &ts, &tsLen));

		//SQL 실행
		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)"));
		*/

		GDBConnectionPool->Push(dbConn);
	}

	// Read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		DBBind<1, 4> dbBind(*dbConn, L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");
		int32 gold = 100;
		dbBind.BindParam(0, gold);

		int32 outId = 0;
		int32 outGold = 0;
		WCHAR outName[100];
		TIMESTAMP_STRUCT outDate = {};

		dbBind.BindCol(0, outId);
		dbBind.BindCol(1, outGold);
		dbBind.BindCol(2, outName);
		dbBind.BindCol(3, outDate);

		ASSERT_CRASH(dbBind.Execute());

		/*
		dbConn->Unbind();

		//넘길 인자
		int32 gold = 100;
		SQLLEN len = 0;
		//넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));

		int32 outId = 0;
		SQLLEN outIdLen = 0;
		ASSERT_CRASH(dbConn->BindCol(1, &outId, &outIdLen));
		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		ASSERT_CRASH(dbConn->BindCol(2, &outGold, &outGoldLen));

		WCHAR outName[100];
		SQLLEN outNameLen = 0;
		ASSERT_CRASH(dbConn->BindCol(3, outName, len32(outName), &outNameLen));

		TIMESTAMP_STRUCT outDate = {};
		SQLLEN outDateLen = 0;
		ASSERT_CRASH(dbConn->BindCol(4, &outDate, &outDateLen));


		//SQL 실행
		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)"));
		*/
		wcout.imbue(locale("kor"));

		while (dbConn->Fetch())
		{
			wcout << "Id : " << outId << ", Gold : " << outGold << ", Name : " << outName << endl;
			wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << endl;
		}

		GDBConnectionPool->Push(dbConn);
	}

	//

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