#pragma once
#include "Creature.h"

class GameSession;
class Room;

class Player : public Creature
{ 
public:
	Player();
	virtual ~Player();

public:
	weak_ptr<GameSession> session;
};

