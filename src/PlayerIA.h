#pragma once
#include "Character.h"
#include "Kart.h"
#include "PowerObject.h"
class PlayerIA
{
private:
	char* playerName;
	Character character;
	Kart kart;
	PowerObject Object;
public:
	PlayerIA();
	~PlayerIA();
};

