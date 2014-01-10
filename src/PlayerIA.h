#pragma once
#include "Character.h"
#include "PowerObject.h"

class PlayerIA
{
private:
	std::string playerName;
	Character &character; //Le personnage choisi au menu
	Kart &kart; //Le kart choisi au menu
	PowerObject *object; //Le pouvoir qu'il a ramassé, =NULL si le joueur n'a pas de pouvoir
public:
	PlayerIA(std::string,Kart&, Character&);
	~PlayerIA();
	
	Kart& getKart(){return kart;}
	Character& getCharacter(){return character;}
	PowerObject* getPower();
	void pickPower(PowerObject& power, int);
	void usePower(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs);
	void stopPower(std::vector<Kart*>& vecKart, int idLanceur);
};

