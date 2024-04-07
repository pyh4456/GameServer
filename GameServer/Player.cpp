#include "pch.h"
#include "Player.h"

Player::Player()
{
	objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_PLAYER);
	_isPlayer = true;
}

Player::~Player()
{
}