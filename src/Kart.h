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
	bool invincible;
	
	Kart();
	Kart(float tourne, float acceleration, float speedmax, float poids);
	~Kart();
	
	void move(int);
	void rotate(int);
	float getSpeed(){return speed;}
	float getSpeedMax(){return speedmax;}
	
	void setSpeed(float s){ speed=s;}
	void setSpeedMax(float s){speedmax=s;}
};

