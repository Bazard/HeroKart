#include "PlayerIA.h"
#include <iostream>

PlayerIA::PlayerIA(std::string name,Kart& kart, Character& character):playerName(name),kart(kart),character(character), object(NULL)
{
}


PlayerIA::~PlayerIA()
{
}

void PlayerIA::pickPower(const PowerObject& power){
	if(!object)
		object=new PowerObject(power);
}

void PlayerIA::usePower(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs){
	if(object){
		object->power(vecKart,idLanceur,tStart, objs);
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