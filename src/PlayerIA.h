#pragma once
#include "Character.h"
#include "Kart.h"
#include "PowerObject.h"

class PlayerIA
{
private:
	std::string playerName;
	Character &character;
	Kart &kart;
	PowerObject *object=NULL;
public:
	PlayerIA(std::string,Kart&, Character&);
	~PlayerIA();
	
	void pickPower(const PowerObject& power);
	void usePower();
};

