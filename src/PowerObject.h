#pragma once
#include "Object3D.h"
#include "Kart.h"

enum typeEnum { BOOST=0, ATK_FRONT=1, ATK_BACK=2, ATK_ALL=3, SHIELD=4, TRAP=5 };

class PowerObject: public Object3D
{
private:
	typeEnum type;
	bool disable; //L'objet n'est pas disponible s'il vient d'être pris, il reapparaitra apres un petit temps
	bool launched;  // indique son etat (lancé ou pas)
	int duration; //sa durée
	int timeOfUse; //le moment quand le pouvoir est lancé
	float stock=0; //stockage de donnée du kart si besoin
	
public:
	PowerObject(typeEnum type, int duration);
	~PowerObject();
	int getDuration(){return duration;}
	int getTimeOfUse(){return timeOfUse;}
	bool isDisable(){return disable;}
	bool isLaunched() {return launched;}
	void power(std::vector<Kart*>& vecKart, int idLanceur,int tStart, std::vector<Object3D*> objs);
	void powerBack(std::vector<Kart*>& vecKart, int idLanceur);
};

