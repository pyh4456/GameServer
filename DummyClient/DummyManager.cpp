#include "pch.h"
#include "DummyManager.h"

DummyManager GDummyManager;

DummyManager::DummyManager()
{ 
}

DummyManager::~DummyManager()
{
}

bool DummyManager::Add(std::shared_ptr<DummySession> session)
{
	WRITE_LOCK;
	_dummys.insert(session);

	return true;
}

bool DummyManager::Remove(std::shared_ptr<DummySession> session)
{
	WRITE_LOCK;
	_dummys.erase(session);

	return true;
}

void DummyManager::Update()
{
	WRITE_LOCK;
	for (std::shared_ptr<DummySession> dummy : _dummys)
	{
		dummy->RandomAction();

		Protocol::C_MOVE MovePkt;
		Protocol::PosInfo* Info = MovePkt.mutable_info();
		
		Protocol::PosInfo* movedPos = dummy->_dummyInfo.mutable_pos_info();

		Info->CopyFrom(dummy->_dummyInfo.pos_info());

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(MovePkt);

		dummy->Send(sendBuffer);
	}
}

void DummyManager::Quit()
{
	WRITE_LOCK;
	for (std::shared_ptr<DummySession> dummy : _dummys)
	{
		dummy->Quit();
	}
}
