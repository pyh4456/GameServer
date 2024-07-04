#pragma once
#include "Object.h"
#include "StateMachine.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

	void RunAi();
	void SendState();

	StateMachine _sm;
};

