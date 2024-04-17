#pragma once
#include "pch.h"
#include "DummySession.h"

class DummyManager
{
public:
	DummyManager();
	virtual ~DummyManager();

public:
	bool Add(std::shared_ptr<DummySession> session);
	bool Remove(std::shared_ptr<DummySession> session);

public:
	void Update();
	void Quit();

private:
	USE_LOCK;
	set<std::shared_ptr<DummySession>> _dummys;
};

extern DummyManager GDummyManager;