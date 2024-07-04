#include "pch.h"
#include "Creature.h"
#include "Room.h"

Creature::Creature()
{
	objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
}

Creature::~Creature()
{
}

void Creature::RunAi()
{
	

	//room.load().lock()->HandleAi();

	//room.load().lock()->DoTimer(1000, &Creature::RunAi);
}

void Creature::SendState()
{
}
