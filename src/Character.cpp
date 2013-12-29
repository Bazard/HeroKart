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
				kart.setAcceleration(kart.getAcceleration()/10);
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
				kart.setAcceleration(kart.getAcceleration()*10);
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

void Character::hitSuperPower(int tStart,std::vector<Kart*>& karts, int idTouche, Kart& kartFrom){ //kart touché
	
	if(karts[idTouche]->invincible || !launched){
		stock.push_back(-1);
		return;
	}
	stock.push_back(idTouche);
	
	switch(hero){
			case KLAUS:
				//Defonce tout
				karts[idTouche]->setSpeed(0.25*karts[idTouche]->getSpeedMax());
				karts[idTouche]->setSpeedMax(0.25*karts[idTouche]->getSpeedMax());
				break;
			case DOUG:
				karts[idTouche]->setSpeed(0.5*karts[idTouche]->getSpeedMax());
				karts[idTouche]->setSpeedMax(0.5*karts[idTouche]->getSpeedMax());
				//Defonce tout
				break;
			case STAN:
				karts[idTouche]->setTourne(karts[idTouche]->getTourne()/10);
				karts[idTouche]->setAcceleration(karts[idTouche]->getAcceleration()/10);
				
				stock.push_back(karts[stock[0]]->getPosition().x-kartFrom.getPosition().x);
				stock.push_back(karts[stock[0]]->getPosition().y-kartFrom.getPosition().y);
				stock.push_back(karts[stock[0]]->getPosition().z-kartFrom.getPosition().z);
				break;
			default:
				break;
		}
}

void Character::hitSuperPowerBack(std::vector<Kart*>& karts){
	if(stock[0]==-1){
		stock.clear();
		return;
	}
		
		switch(hero){
			case KLAUS:
				karts[stock[0]]->setSpeed(4*karts[stock[0]]->getSpeedMax());
				karts[stock[0]]->setSpeedMax(4*karts[stock[0]]->getSpeedMax());
				break;
			case DOUG:
				karts[stock[0]]->setSpeed(2*karts[stock[0]]->getSpeedMax());
				karts[stock[0]]->setSpeedMax(2*karts[stock[0]]->getSpeedMax());
				break;
			case STAN:
				karts[stock[0]]->setTourne(karts[stock[0]]->getTourne()*10);
				karts[stock[0]]->setAcceleration(karts[stock[0]]->getAcceleration()*10);
				break;
			default:
				break;
		}
	stock.clear();
}

void Character::continuousHitSuperPower(std::vector<Kart*>& karts, Kart& kart){
	if(stock.empty())
		return;
		
	if(stock[0]==-1)
		return;
		
		switch(hero){
			case STAN:
				//Deplacement du kart dependant de celui de Stan
				karts[stock[0]]->setPosition(kart.getPosition().x+stock[1],kart.getPosition().y+stock[2],kart.getPosition().z+stock[3]);
				karts[stock[0]]->setAngle(kart.getAngle());
				break;
			default:
				break;
		}
}