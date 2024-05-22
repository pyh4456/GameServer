#pragma once

class ObjectUtils
{
public:
	static PlayerRef CreatePlayer(GameSessionRef session);
	static ObjectRef CreateProjectile();
	static MonsterRef CreateMonster();

private:
	static atomic<int64> s_idGenerator;
};

 