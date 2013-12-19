#include "Kart.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#define EPSILON 0.00001

Kart::Kart():tourne(2.f), acceleration(0.1f), speed(0),speedmax(0.5), back(false){
	dir.z=1;
}

Kart::Kart(float tourne, float acceleration, float speedmax) :tourne(tourne), acceleration(acceleration), speedmax(speedmax), speed(0), back(false)
{
	dir.z=1;
}


Kart::~Kart()
{
}

void Kart::move(int sens){
	if(sens>0){
		back=false;
		if(speed<=speedmax)
			speed+=acceleration;
		}
	else if(sens<0){
		
		if(speed > 2*acceleration)
			speed-=2*acceleration;
			
		else if(speed>=-speedmax/2.f){
			back=true;
			speed-=acceleration;
		}
		}
		
	else {
		if(speed>0) speed-=acceleration/2.f;
		else if(speed<0) speed+=acceleration/2.f;
	}
	
	if(speed < EPSILON && speed > -EPSILON){
		back=false;
		speed = 0;
	}
	
	std::cout << "speed" << speed << std::endl;
	pos+=speed*dir;
	
}

void Kart::rotate(int sens){
	if(sens>=0){
		dir=glm::normalize(glm::rotate(dir,tourne,glm::vec3(0,1,0)));
		angle+=tourne;
	}
	else {
		dir=glm::normalize(glm::rotate(dir,-tourne,glm::vec3(0,1,0)));
		angle-=tourne;
	}
}
