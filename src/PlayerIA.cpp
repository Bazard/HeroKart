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

void PlayerIA::usePower(){
	if(object){
		std::cout << "Pan" << std::endl;
		delete(object);
		object=NULL;
		}
	else
		std::cout << "Pas d'objet" << std::endl;
}