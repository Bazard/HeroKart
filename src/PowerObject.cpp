#include "PowerObject.h"


PowerObject::PowerObject(typeEnum type, int duration):type(type), duration(duration), disable(true), launched(false)
{
}


PowerObject::~PowerObject()
{
}

void PowerObject::power(std::vector<Kart*>& vecKart, int idLanceur, int tStart, std::vector<Object3D*> objs){
		switch(type){
			case BOOST:
					stock=vecKart[idLanceur]->getSpeedMax();
					vecKart[idLanceur]->setSpeed(2*vecKart[idLanceur]->getSpeedMax());
					vecKart[idLanceur]->setSpeedMax(3*vecKart[idLanceur]->getSpeedMax());
					break;
			case ATK_FRONT:
					
					break;
			case ATK_BACK:
			
					break;
			case ATK_ALL:
					break;
			case SHIELD:
					break;
			case TRAP:
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
					break;
			case ATK_FRONT:
					break;
			case ATK_BACK:
					break;
			case ATK_ALL:
					break;
			case SHIELD:
					break;
			case TRAP:
					break;
			default:
					//Cheat
					break;
		}
	}