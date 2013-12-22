#include "PowerObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

PowerObject::PowerObject(typeEnum type, int duration):Object3D(), type(type), duration(duration), launched(false) 
{
}

PowerObject::PowerObject(const PowerObject& obj):Object3D(), type(obj.type), duration(obj.duration), launched(false) 
{
}

PowerObject::~PowerObject()
{
}

void PowerObject::power(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*>& objs){
		switch(type){
			case BOOST:
					visible=false; //Car ce n'est pas un objet visible
					stock=vecKart[idLanceur]->getSpeedMax();
					vecKart[idLanceur]->setSpeed(2*vecKart[idLanceur]->getSpeedMax());
					vecKart[idLanceur]->setSpeedMax(3*vecKart[idLanceur]->getSpeedMax());
					break;
			case ATK_FRONT:
					visible=true;
					break;
			case ATK_BACK:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/EarthMap.jpg"); //A changer par une mine
					pos=vecKart[idLanceur]->getPosition();
					id=objs.size(); // Stock l'id du powerObject en cours
					objs.push_back(this);
					
					break;
			case ATK_ALL:
					visible=false;
					for(int i=0;i<vecKart.size();++i){
						if(idLanceur!=i){
							vecKart[i]->setSpeed(0.5*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setSpeedMax(0.5*vecKart[idLanceur]->getSpeedMax());
							vecKart[i]->setScale(glm::vec3(0.5*vecKart[i]->getScale().x,0.5*vecKart[i]->getScale().y,0.5*vecKart[i]->getScale().z));
						}
					}
					break;
			case SHIELD:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/MoonMap.png"); //A changer par un bouclier
					pos=vecKart[idLanceur]->getPosition();
					vecKart[idLanceur]->invincible=true;
					break;
			case TRAP:
					visible=true;
					sphere(1,32,16);
					build();
					LoadTexture("../textures/triforce.jpg"); //A changer par un piege
					pos=vecKart[idLanceur]->getPosition();
					id=objs.size(); // Stock l'id du powerObject en cours
					objs.push_back(this);
					break;
			default:
					//Cheat
					break;
		}
		timeOfUse=tStart;
		launched=true;
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
						if(idLanceur!=i){
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