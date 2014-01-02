#include "PlayerIA.h"
#include <iostream>

PlayerIA::PlayerIA(std::string name,Kart& kart, Character& character):playerName(name),kart(kart),character(character), object(NULL)
{
}


PlayerIA::~PlayerIA()
{
}

void PlayerIA::pickPower(PowerObject& power, int tStart){
	if(!object){
		object=new PowerObject(power);
		power.setTimeOfUse(tStart);
	}
}

void PlayerIA::usePower(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs){
	if(object){
		if(object->power(vecKart,idLanceur,tStart, objs)==1)
			object=NULL;
		}
	else
		std::cout << "Pas d'objet" << std::endl;
}

void PlayerIA::stopPower(std::vector<Kart*>& vecKart, int idLanceur){
	if(object){
		object->powerBack(vecKart,idLanceur);
		object=NULL;
		}
	else
		std::cout << "Pas d'objet" << std::endl;
}

PowerObject* PlayerIA::getPower(){
		return object;
}