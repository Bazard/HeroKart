#include "PowerObject.h"


PowerObject::PowerObject(typeEnum type):type(type)
{
}


PowerObject::~PowerObject()
{
}

void PowerObject::power(std::vector<Kart*>& vecKart, int idLanceur){
		switch(type){
			case BOOST:
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
	}