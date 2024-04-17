#pragma once
#include "pch.h"

class Dummy
{
public:
	Dummy();
	~Dummy();
	
	void enter();
	void leave();
	void move();

	void randomAction();

private:
	Protocol::ObjectInfo _dummyInfo;
	std::weak_ptr<DummySession> _session;
};

