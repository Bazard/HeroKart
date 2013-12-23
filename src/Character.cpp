#include "Character.h"
#include <iostream>

Character::Character(Hero hero, int duration):hero(hero), reloadtime(10000),timeOfUse(-10000), duration(duration), launched(false)
{
}


Character::~Character()
{
}

bool Character::isPowerReady(int tStart){
	if(reloadtime+timeOfUse+duration < tStart)
		return true;
	return false;
}

void Character::useSuperPower(int tStart, Kart& kart){
	if(isPowerReady(tStart)){
		timeOfUse=tStart;
		launched=true;
		switch(hero){
			case JOHN:
				kart.invincible=true;
				break;
			case KLAUS:
				break;
			case DOUG:
				break;
			case CANADA:
				kart.intouchable=true;
				kart.setPosition(kart.getPosition().x,kart.getPosition().y+3,kart.getPosition().z);
				break;
			case BURT:
				break;
			case MCKORMACK:
				break;
			case STEVE:
				break;
			case VALUR:
				break;
		}
		
	}
	else{
		std::cout << "Reloading" << std::endl;
	}
}

void Character::useSuperPowerBack(Kart& kart){
	launched=false;
	switch(hero){
			case JOHN:
				kart.invincible=false;
				break;
			case KLAUS:
				break;
			case DOUG:
				break;
			case CANADA:
				kart.intouchable=false;
				kart.setPosition(kart.getPosition().x,kart.getPosition().y-3.f,kart.getPosition().z);
				break;
			case BURT:
				break;
			case MCKORMACK:
				break;
			case STEVE:
				break;
			case VALUR:
				break;
		}
}

bool Character::isPerimed(int tStart){
	if(timeOfUse+duration < tStart)
		return true;
	return false;
}

void Character::hitSuperPower(int tStart,Kart& kart){

}

void Character::hitSuperPowerBack(Kart& kart){

}