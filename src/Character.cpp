#include "Character.h"
#include "PowerObject.h"
#include <iostream>

Character::Character(Hero hero, int duration):hero(hero), reloadtime(10000),timeOfUse(-10000), duration(duration), launched(false), stock(0.f)
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


void Character::useSuperPower(int tStart, Kart& kart, std::vector<Object3D*>& mapObjects){
	if(isPowerReady(tStart)){
		timeOfUse=tStart;
		launched=true;
		PowerObject* obj;
		
		switch(hero){
			case JOHN:
				kart.invincible=true;
				break;
			case KLAUS:
				//Jouer le son "Pinage"
				break;
			case DOUG:
				kart.setSpeed(kart.getSpeedMax());
				kart.setTourne(kart.getTourne()/10);
				stock=kart.getAcceleration();
				kart.setAcceleration(0.f);
				break;
			case CANADA:
				kart.intouchable=true;
				kart.setPosition(kart.getPosition().x,kart.getPosition().y+3,kart.getPosition().z);
				break;
			case BURT:
				obj=new PowerObject(ATK_BACK, 10000);
				obj->visible=true;
				obj->sphere(1,32,16); //A changer par une flaque
				obj->build();
				obj->LoadTexture("../textures/sky.jpg"); //A changer par une texture d'acide
				obj->setPosition(kart.getPosition());
				obj->setDirection(kart.getDirection());
				obj->setAngle(kart.getAngle());
				mapObjects.push_back(obj);
				break;
			case MCKORMACK:
				kart.setPosition(kart.getPosition().x+20*kart.getDirection().x, kart.getPosition().y+20*kart.getDirection().y, kart.getPosition().z+20*kart.getDirection().z);
				break;
			case STEVE:
				obj=new PowerObject(ATK_FRONT, 10000);
				obj->visible=true;
				obj->sphere(1,32,16);
				obj->build();
				obj->LoadTexture("../textures/sky.jpg"); //A changer par une boule de feu
				obj->setPosition(kart.getPosition());
				obj->setDirection(kart.getDirection());
				obj->setAngle(kart.getAngle());
				mapObjects.push_back(obj);
				break;
			case STAN:
				break;
			default:
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
				kart.setTourne(kart.getTourne()*10);
				kart.setAcceleration(stock);
				break;
			case CANADA:
				kart.intouchable=false;
				kart.setPosition(kart.getPosition().x,kart.getPosition().y-3.f,kart.getPosition().z);
				break;
			case BURT:
				break;
			case STEVE:
				break;
			case STAN:
				break;
			default:
				break;
		}
}

bool Character::isPerimed(int tStart){
	if(timeOfUse+duration < tStart)
		return true;
	return false;
}

void Character::hitSuperPower(int tStart,Kart& kart){ //kart touché
	stock=0;
	if(kart.invincible){
		stock=-1;
		return;
	}
		
	switch(hero){
			case KLAUS:
				//Defonce tout
				kart.setSpeed(0.25*kart.getSpeedMax());
				kart.setSpeedMax(0.25*kart.getSpeedMax());
				break;
			case DOUG:
				kart.setSpeed(0.5*kart.getSpeedMax());
				kart.setSpeedMax(0.5*kart.getSpeedMax());
				//Defonce tout
				break;
			case STAN:
				//Deplacement du kart dependant de celui de Stan
				break;
			default:
				break;
		}
}

void Character::hitSuperPowerBack(Kart& kart){
	if(stock==-1)
		return;
		
		switch(hero){
			case KLAUS:
				//Defonce tout
				kart.setSpeed(4*kart.getSpeedMax());
				kart.setSpeedMax(4*kart.getSpeedMax());
				break;
			case DOUG:
				kart.setSpeed(2*kart.getSpeedMax());
				kart.setSpeedMax(2*kart.getSpeedMax());
				//Defonce tout
				break;
			case STAN:
				//Deplacement du kart dependant de celui de Stan
				break;
			default:
				break;
		}
}