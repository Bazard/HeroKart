#pragma once
#include "Race.h"
#include "PlayerIA.h"

class Game
{
private:
	Race race;
	PlayerIA players[8];
public:
	Game();
	~Game();
};

