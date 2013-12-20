#pragma once
#include "Object3D.h"

enum typeEnum { BOOST=0, ATK_FRONT=1, ATK_BACK=2, ATK_ALL=3, SHIELD=4, TRAP=5 };

class PowerObject: public Object3D
{
private:
	typeEnum type;
	bool disable; //L'objet n'est pas disponible s'il vient d'être pris, il reapparaitra apres un petit temps
public:
	PowerObject(typeEnum type);
	~PowerObject();
};

