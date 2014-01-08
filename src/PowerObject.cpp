#include "PowerObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PowerObject::PowerObject(typeEnum type, int duration):Object3D(), type(type), duration(duration), launched(false), timeOfUse(-1), pick(true)
{
}

PowerObject::PowerObject(const PowerObject& obj):Object3D(), type(obj.type), duration(obj.duration), launched(false), timeOfUse(-1), pick(false)
{
}

PowerObject::~PowerObject()
{
}

int PowerObject::power(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs){
		int retour;
		if(launched)
			return 0;
			
		
		switch(type){
			case BOOST:
					visible=false; //Car ce n'est pas un objet visible
					stock.push_back(vecKart[idLanceur]->getSpeedMax());
					vecKart[idLanceur]->setSpeed(1.5*vecKart[idLanceur]->getSpeedMax());
					vecKart[idLanceur]->setSpeedMax(2*vecKart[idLanceur]->getSpeedMax());
					timeOfUse=tStart;
					retour=0;
					break;
					
			case ATK_FRONT:
					visible=true;
					LoadObjFromFile("../models/Missile.obj");	
					build();
					LoadTexture("../textures/TexMissile.jpg"); //A changer par une mine
					pos=vecKart[idLanceur]->getPosition()+glm::vec3(3*vecKart[idLanceur]->getDirection().x,3*vecKart[idLanceur]->getDirection().y,3*vecKart[idLanceur]->getDirection().z);
					dir=vecKart[idLanceur]->getDirection();
					angle=vecKart[idLanceur]->getAngle();
					sca=glm::vec3(3);
					objs.push_back(this);
					retour=1;
					break;
					
			case ATK_BACK:
					visible=true;
					LoadObjFromFile("../models/Mine.obj");	
					build();
					LoadTexture("../textures/TexMine.jpg"); //A changer par une mine
					sca=glm::vec3(2);
					pos=vecKart[idLanceur]->getPosition()-glm::vec3(3*vecKart[idLanceur]->getDirection().x,3*vecKart[idLanceur]->getDirection().y,3*vecKart[idLanceur]->getDirection().z);
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
						else if(idLanceur!=i && vecKart[i]->invincible){
							std::cout << "John" << i << std::endl;
							stock.push_back(i);
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
					pos=vecKart[idLanceur]->getPosition()-glm::vec3(3*vecKart[idLanceur]->getDirection().x,3*vecKart[idLanceur]->getDirection().y,3*vecKart[idLanceur]->getDirection().z);
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
					vecKart[idLanceur]->setSpeedMax(stock[0]);
					delete(this);
					break;
					
			case ATK_FRONT:
					break;
					
			case ATK_BACK:
					break;
					
			case ATK_ALL:
				bool inv;
					for(int i=0;i<vecKart.size();++i){
						if(idLanceur!=i){
							inv=false;
							for(int j=0;j<stock.size();++j){
								if(stock[j]==i) inv=true;
							}
							if(!inv){
								vecKart[i]->setSpeedMax(2*vecKart[i]->getSpeedMax());
								vecKart[i]->setScale(glm::vec3(2*vecKart[i]->getScale().x,2*vecKart[i]->getScale().y,2*vecKart[i]->getScale().z));
							}
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
		stock.clear();
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

int PowerObject::hitKart(Kart& kart, int id, int tStart){
		
	if(pick){
		stock.push_back(-1);
		return 1;
	}
	
	visible=false;
	if(kart.invincible){
		stock.push_back(-1);
		return 1;
	}
		stock.push_back(id);
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
	
	return 1;
}

void PowerObject::hitKartBack(std::vector<Kart*>& karts){
	
	if(stock[0]==-1){
		stock.clear();
		return;
	}
	
	switch(type){ //A varier si l'on veut
		case ATK_FRONT:
			karts[stock[0]]->setSpeedMax(4*karts[stock[0]]->getSpeedMax());
			break;
		case ATK_BACK:
			karts[stock[0]]->setSpeedMax(2*karts[stock[0]]->getSpeedMax());
			break;
		case TRAP:
			karts[stock[0]]->setSpeedMax(2*karts[stock[0]]->getSpeedMax());
			break;
		default:
			break;
	}
	stock.clear();
}