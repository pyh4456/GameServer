#pragma once
#include "Object.h"


class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

	void RunAi();
	void GotHit();

	void SetState(Protocol::AiState state);
	void SetTarget(Protocol::PosInfo *target);

	Protocol::AiState GetState() { return _state; }
	Protocol::PosInfo GetTarget() { return _target; }

private:
	Protocol::AiState _state;
	Protocol::PosInfo _target;
};