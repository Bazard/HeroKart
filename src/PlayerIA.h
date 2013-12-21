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
	
	PowerObject* getPower();
	void pickPower(const PowerObject& power);
	void usePower(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs);
	void stopPower(std::vector<Kart*>& vecKart, int idLanceur);
};

