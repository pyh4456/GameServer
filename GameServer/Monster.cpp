#include "pch.h"
#include "Monster.h"

Monster::Monster()
{
	objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_ENEMY);
}

Monster::~Monster()
{
}
