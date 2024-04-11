#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->objectInfo->set_object_id(newId);
	player->posInfo->set_object_id(newId);

	player->session = session;
	session->player.store(player);

	return player;
}

ObjectRef ObjectUtils::CreateProjectile()
{
	const int64 newId = s_idGenerator.fetch_add(1);

	ObjectRef object = make_shared<Object>();
	object->objectInfo->set_object_id(newId);
	object->posInfo->set_object_id(newId);

	return object;
}

MonsterRef ObjectUtils::CreateMonster()
{
	const int64 newId = s_idGenerator.fetch_add(1);

	MonsterRef monster = make_shared<Monster>();
	monster->objectInfo->set_object_id(newId);
	monster->posInfo->set_object_id(newId);
	monster->objectInfo->set_object_type(Protocol::OBJECT_TYPE_ENEMY);

	return monster;
}
