#include "PowerObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PowerObject::PowerObject(typeEnum type, int duration):Object3D(), type(type), duration(duration), launched(false), timeOfUse(-1), stock(0)
{
}

PowerObject::PowerObject(const PowerObject& obj):Object3D(), type(obj.type), duration(obj.duration), launched(false), timeOfUse(-1), stock(0)
{
}

PowerObject::~PowerObject()
{
}

int PowerObject::power(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs){
		int retour;
		switch(type){
			case BOOST:
					visible=false; //Car ce n'est pas un objet visible
					stock=vecKart[idLanceur]->getSpeedMax();
					vecKart[idLanceur]->setSpeed(2*vecKart[idLanceur]->getSpeedMax());
					vecKart[idLanceur]->setSpeedMax(3*vecKart[idLanceur]->getSpeedMax());
					timeOfUse=tStart;
					retour=0;
					break;
					
			case ATK_FRONT:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/sky.jpg"); //A changer par une mine
					pos=vecKart[idLanceur]->getPosition();
					dir=vecKart[idLanceur]->getDirection();
					angle=vecKart[idLanceur]->getAngle();
					objs.push_back(this);
					retour=1;
					break;
					
			case ATK_BACK:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/EarthMap.jpg"); //A changer par une mine
					pos=vecKart[idLanceur]->getPosition();
					objs.push_back(this);
					retour=1;
					break;
					
			case ATK_ALL:
					visible=false;
					for(int i=0;i<vecKart.size();++i){
						if(idLanceur!=i && !vecKart[i]->invincible){
							vecKart[i]->setSpeed(0.5*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setSpeedMax(0.5*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setScale(glm::vec3(0.5*vecKart[i]->getScale().x,0.5*vecKart[i]->getScale().y,0.5*vecKart[i]->getScale().z));
						}
					}
					timeOfUse=tStart;
					retour=0;
					break;
					
			case SHIELD:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/MoonMap.png"); //A changer par un bouclier
					pos=vecKart[idLanceur]->getPosition();
					vecKart[idLanceur]->invincible=true;
					timeOfUse=tStart;
					retour=0;
					break;
					
			case TRAP:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/triforce.jpg"); //A changer par un piege
					pos=vecKart[idLanceur]->getPosition();
					objs.push_back(this);
					retour=1;
					break;
					
			default:
					//Cheat
					break;
		}
		
		launched=true;
		return retour;
	}
	
void PowerObject::powerBack(std::vector<Kart*>& vecKart, int idLanceur){
		switch(type){
			case BOOST:
					vecKart[idLanceur]->setSpeed(stock);
					vecKart[idLanceur]->setSpeedMax(stock);
					delete(this);
					break;
					
			case ATK_FRONT:
					break;
					
			case ATK_BACK:
					break;
					
			case ATK_ALL:
					for(int i=0;i<vecKart.size();++i){
						if(idLanceur!=i && !vecKart[i]->invincible){
							vecKart[i]->setSpeed(2*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setSpeedMax(2*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setScale(glm::vec3(2*vecKart[i]->getScale().x,2*vecKart[i]->getScale().y,2*vecKart[i]->getScale().z));
						}
					}
					delete(this);
					break;
					
			case SHIELD:
					vecKart[idLanceur]->invincible=true;
					delete(this);
					break;
					
			case TRAP:
					break;
					
			default:
					//Cheat
					break;
		}
		
	}

bool PowerObject::withKart(){
	if(type==SHIELD)
		return true;
	return false;
}

void PowerObject::movePower(){
	switch(type){
		case ATK_FRONT:
			pos+=glm::vec3(2*dir.x,2*dir.y,2*dir.z);
			break;
			
		case TRAP:
			angle+=1;
			break;
			
		case ATK_BACK:
			angle+=1;
			break;
	}
}

bool PowerObject::tooFar(){
	if(pos.x>10000 || pos.y>10000 || pos.z>10000 || pos.x<-10000 || pos.y<-10000 || pos.z<-10000)
		return true;
	return false;
}

bool PowerObject::isPerimed(int tStart){
	if(timeOfUse<=0)
		return false;

	if(duration+timeOfUse < tStart)
		return true;
	
	return false;
}

void PowerObject::hitKart(Kart& kart, int id, int tStart){
	if(kart.invincible){
		stock==-1;
		return;
	}
		stock=id;
		timeOfUse=tStart;
		
	switch(type){ //A varier si l'on veut
		case ATK_FRONT:
			kart.setSpeed(0.25*kart.getSpeedMax());
			kart.setSpeedMax(0.25*kart.getSpeedMax());
			break;
		case ATK_BACK:
			kart.setSpeed(0.5*kart.getSpeedMax());
			kart.setSpeedMax(0.5*kart.getSpeedMax());
			break;
		case TRAP:
			kart.setSpeed(0.5*kart.getSpeedMax());
			kart.setSpeedMax(0.5*kart.getSpeedMax());
			break;
		default:
			break;
	}
}

void PowerObject::hitKartBack(std::vector<Kart*>& karts){
	if(stock==-1){
		return;
	}
		
	switch(type){ //A varier si l'on veut
		case ATK_FRONT:
			karts[stock]->setSpeed(4*karts[stock]->getSpeedMax());
			karts[stock]->setSpeedMax(4*karts[stock]->getSpeedMax());
			break;
		case ATK_BACK:
			karts[stock]->setSpeed(2*karts[stock]->getSpeedMax());
			karts[stock]->setSpeedMax(2*karts[stock]->getSpeedMax());
			break;
		case TRAP:
			karts[stock]->setSpeed(2*karts[stock]->getSpeedMax());
			karts[stock]->setSpeedMax(2*karts[stock]->getSpeedMax());
			break;
		default:
			break;
	}
}