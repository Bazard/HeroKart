#pragma once
#include "Object3D.h"

class Kart: public Object3D
{
private:
	float tourne;
	float acceleration;
	float speedmax;
	float speed;
	float poids;
	
public:
	bool back;

	Kart();
	Kart(float tourne, float acceleration, float speedmax, float poids);
	~Kart();
	
	void move(int);
	void rotate(int);
};

