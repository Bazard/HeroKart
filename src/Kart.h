#pragma once
#include "Object3D.h"
#include "Node.h"

class PowerObject;

class Kart: public Object3D
{
private:
	float tourne;
	float acceleration;
	float speedmax;
	float speed;
	float poids;
	Node* nodeTo;
	
public:
	bool back;
	bool invincible;
	bool intouchable;
	
	Kart();
	Kart(float tourne, float acceleration, float speedmax, float poids);
	~Kart();
	
	void move(int);
	void rotate(int);
	float getSpeed(){return speed;}
	float getSpeedMax(){return speedmax;}
	
	void setSpeed(float s){ speed=s;}
	void setSpeedMax(float s){speedmax=s;}
	
	void setTourne(float t){ tourne=t;}
	float getTourne(){ return tourne;}
	
	void setAcceleration(float a){ 	acceleration=a;}
	float getAcceleration(){ return acceleration;}
	
	void setNodeTo(Node* node){nodeTo=node;}
	
	void moveIA(std::vector<Object3D*>& mapObjects,std::vector<PowerObject*>& powObjects, std::vector<Kart*>& Karts,PowerObject* power); //power = Le pouvoir du kart
};

