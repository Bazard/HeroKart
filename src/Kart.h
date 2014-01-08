#pragma once
#include "Object3D.h"
#include "Node.h"

class PowerObject;

enum Hero { JOHN=0, KLAUS=1, DOUG=2, CANADA=3, BURT=4, MCKORMACK=5, STEVE=6, STAN=7, JENNIFER=8 };

class Kart: public Object3D
{
private:
	float tourne;
	float acceleration;
	float speedmax;
	float speed;
	float poids;
	Node* nodeTo;
	unsigned int nbNodesPassed;
	unsigned int rank;
	
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

	unsigned int getNbNodesPassed(){return nbNodesPassed;}

	void setRank(unsigned int r){rank=r;}
	unsigned int getRank(){return rank;}
	void incrRank();
	void decrRank();
	
	void setNodeTo(Node* node){nodeTo=node;}
	void crossANode(void);
	float distanceToNextNode(void);
	
	int moveIA(std::vector<Object3D*>& mapObjects,std::vector<PowerObject*>& powObjects, std::vector<Kart*>& Karts,PowerObject* power, Hero hero, bool powerReady); //power = Le pouvoir du kart
	float getAngle2Vec(glm::vec3 vec2, glm::vec3 vec1);
	float getAngle2Vec(float x, float z);
};

