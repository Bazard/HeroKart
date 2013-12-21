#include "PlayerIA.h"
#include <iostream>

PlayerIA::PlayerIA(std::string name,Kart& kart, Character& character):playerName(name),kart(kart),character(character)
{
}


PlayerIA::~PlayerIA()
{
}

void PlayerIA::pickPower(const PowerObject& power){
	object=new PowerObject(power);
}

void PlayerIA::usePower(std::vector<Kart*>& vecKart, int idLanceur, int tStart){
	if(object){
		object->power(vecKart,idLanceur,tStart);
		}
	else
		std::cout << "Pas d'objet" << std::endl;
}

void PlayerIA::stopPower(std::vector<Kart*>& vecKart, int idLanceur){
	if(object){
		object->powerBack(vecKart,idLanceur);
		}
	else
		std::cout << "Pas d'objet" << std::endl;
}

void PlayerIA::dropPower(){
	if(object){
		delete(object);
		object=NULL;
	}
}

PowerObject* PlayerIA::getPower(){
		return object;
}