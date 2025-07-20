#include "pch.h"
#include "Creature.h"
#include "Room.h"

Creature::Creature()
{
	objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
	_state = Protocol::AISTATE_IDLE;
	_target = *posInfo;
}

Creature::~Creature()
{
}

void Creature::RunAi()
{
	float distance = 0;

	switch (_state)
	{
	case Protocol::AISTATE_IDLE:
		if (posInfo->x() == _target.x() && posInfo->y() == _target.y() && posInfo->z() == _target.z())
			break;

		distance += (posInfo->x() - _target.x()) * (posInfo->x() - _target.x());
		distance += (posInfo->y() - _target.y()) * (posInfo->y() - _target.y());
		distance += (posInfo->z() - _target.z()) * (posInfo->z() - _target.z());

		if (distance <= 62'500)
			_state = Protocol::AISTATE_ATTACK;
			break;

		if (distance <= 1'000'000)
			_state = Protocol::AISTATE_MOVE;
			break;

		break;

	case Protocol::AISTATE_MOVE:
		_state = Protocol::AISTATE_ATTACK;
		break;

	case Protocol::AISTATE_ATTACK:
		_state = Protocol::AISTATE_IDLE;
		break;

	case Protocol::AISTATE_RUNAWAY:
		_state = Protocol::AISTATE_IDLE;
		break;
	}
}

void Creature::GotHit()
{
	switch (_state)
	{
	case Protocol::AISTATE_IDLE:
		_state = Protocol::AISTATE_RUNAWAY;
		break;

	case Protocol::AISTATE_MOVE:
		_state = Protocol::AISTATE_RUNAWAY;
		break;

	default:
		break;
	}
}

void Creature::SetState(Protocol::AiState state)
{
	_state = state;
}

void Creature::SetTarget(Protocol::PosInfo *target)
{
	_target.CopyFrom(*target);
}
