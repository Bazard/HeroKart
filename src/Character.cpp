#include "Character.h"
#include <iostream>

Character::Character():reloadtime(10000),timeOfUse(-10000)
{
}


Character::~Character()
{
}

bool Character::isPowerReady(int tStart){
	if(reloadtime+timeOfUse < tStart)
		return true;
	return false;
}

void Character::useSuperPower(int tStart){
	if(isPowerReady(tStart)){
		std::cout << "Boom" << std::endl;
		timeOfUse=tStart;
	}
	else{
		std::cout << "Reloading" << std::endl;
	}
}