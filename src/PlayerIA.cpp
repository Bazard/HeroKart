#include "PlayerIA.h"


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
	delete(object);
}