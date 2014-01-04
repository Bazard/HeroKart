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
				obj->setPick(false);
				obj->LoadObjFromFile("../models/Flaque.obj");
				obj->build();
				obj->LoadTexture("../textures/TexFlaque.jpg");
				obj->setHitbox(glm::vec3(3));
				obj->setScale(glm::vec3(3));
				obj->setPosition(kart.getPosition()-glm::vec3(6*kart.getDirection().x,6*kart.getDirection().y,6*kart.getDirection().z));
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
				obj->setPick(false);
				obj->LoadObjFromFile("../models/Boule.obj");
				obj->build();
				obj->setHitbox(glm::vec3(3));
				obj->setScale(glm::vec3(10));
				obj->LoadTexture("../textures/TexBoule.jpg");
				obj->setPosition(kart.getPosition()+glm::vec3(3*kart.getDirection().x,3*kart.getDirection().y,3*kart.getDirection().z));
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
		
	if(karts[idTouche]->invincible || !launched || !stock.empty()){
		return;
	}
	
	stock.push_back(idTouche);
	
	switch(hero){
			case KLAUS:
				//Defonce tout
				karts[idTouche]->setAngle(karts[idTouche]->getAngle()-180);
				karts[idTouche]->setDirection(-karts[idTouche]->getDirection().x,0,-karts[idTouche]->getDirection().z);
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
				float var;
				var=karts[stock[0]]->getPosition().x-kartFrom.getPosition().x;
				if(var<0) stock.push_back(var-1);
				else stock.push_back(var+1);
				
				stock.push_back(karts[stock[0]]->getPosition().y-kartFrom.getPosition().y);
				
				var=karts[stock[0]]->getPosition().z-kartFrom.getPosition().z;
				if(var<0) stock.push_back(var-1);
				else stock.push_back(var+1);
				
				break;
			default:
				break;
		}
}

void Character::hitSuperPowerBack(std::vector<Kart*>& karts){

	if(stock.empty()){
		return;
	}
		
		switch(hero){
			case KLAUS:
				karts[stock[0]]->setSpeedMax(4*karts[stock[0]]->getSpeedMax());
				break;
			case DOUG:
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
		
		switch(hero){
			case STAN:
				//Deplacement du kart dependant de celui de Stan
				karts[stock[0]]->setPosition(kart.getPosition().x+stock[1],kart.getPosition().y+stock[2],kart.getPosition().z+stock[3]);
				karts[stock[0]]->setAngle(kart.getAngle());
				karts[stock[0]]->setDirection(kart.getDirection());
				break;
			default:
				break;
		}
}