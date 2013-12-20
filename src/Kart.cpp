#include "Kart.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#define EPSILON 0.00001

Kart::Kart():tourne(2.f), acceleration(0.1f), speed(0),speedmax(0.5), back(false), poids(5){
	dir.z=1;
}

Kart::Kart(float tourne, float acceleration, float speedmax, float poids) :tourne(tourne), acceleration(acceleration), speedmax(speedmax), speed(0), back(false), poids(poids)
{
	dir.z=1;
}


Kart::~Kart()
{
}

void Kart::move(int sens){
	if(sens>0){
		back=false;
		//En avant
		if(speed<=speedmax)
			speed+=acceleration;
		}
	else if(sens<0){
		//Freine
		if(speed > 2*acceleration)
			speed-=2*acceleration;
		//Marche Arriere
		else if(speed>=-speedmax/2.f){
			if(speed<-0.1)
				back=true;
			speed-=acceleration;
		}
		}
		
	else { //Idle (Touche a rien)
		if(speed>0) speed-=acceleration/2.f;
		else if(speed<0) speed+=acceleration/2.f;
	}
	
	if(speed < EPSILON && speed > -EPSILON){
		back=false;
		speed = 0;
	}
	
	pos+=speed*dir;
	
}

void Kart::rotate(int sens){
	float idle=0.05;
	if(speed < idle && speed > -idle)
		return;
		
		float coeff=1/((sqrt(speed*speed))/speedmax)*0.5; //Rotation en fonction de la vitesse
		if(coeff>5) coeff=3;
		
		// std::cout << "tourne" << tourne << std::endl;
		// std::cout << "coeff" << coeff << std::endl;
		// std::cout << "speed" << speed << std::endl;
		// std::cout << std::endl;
	
		
		
	if(sens>=0){
		if(speed>0){
			dir=glm::normalize(glm::rotate(dir,tourne*coeff,glm::vec3(0,1,0)));
			angle+=tourne*coeff;
		}
		else {
			dir=glm::normalize(glm::rotate(dir,-tourne*coeff,glm::vec3(0,1,0)));
			angle-=tourne*coeff;
		}
	}
	else {
		if(speed>0){
			dir=glm::normalize(glm::rotate(dir,-tourne*coeff,glm::vec3(0,1,0)));
			angle-=tourne*coeff;
		}
		else {
			dir=glm::normalize(glm::rotate(dir,tourne*coeff,glm::vec3(0,1,0)));
			angle+=tourne*coeff;
		}
	}
}
