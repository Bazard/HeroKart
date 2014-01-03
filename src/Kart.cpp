#include "Kart.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#define EPSILON 0.00001

Kart::Kart():tourne(2.f), acceleration(0.1f), speed(0),speedmax(0.5), back(false), poids(5),invincible(false),intouchable(false),nodeTo(NULL){
	dir.z=1;
}

Kart::Kart(float tourne, float acceleration, float speedmax, float poids) :tourne(tourne), acceleration(acceleration), speedmax(speedmax), speed(0), back(false), poids(poids),invincible(false),intouchable(false),nodeTo(NULL)
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
	// angle=fmod(angle,360);
	// if(angle>180) angle-=360;
	// else if(angle<-180) angle+=360;
}

void Kart::moveIA(std::vector<Object3D*>& mapObjects){
	float idle=0.05;
	float angleNode,x,z;
	bool marchearriere=false, droite=false, gauche=false, obstacle=false;
	
	//Repere les obstacles
	for (std::vector<Object3D*>::iterator it = mapObjects.begin()+1 ; it != mapObjects.end(); ++it){
		x=(*it)->getPosition().x-pos.x;
		z=(*it)->getPosition().z-pos.z;
		
		angleNode=atan(x/z)*180/M_PI-angle;
		if(z<0){ 
				if(angleNode<0) angleNode+=180;
				else angleNode-=180;
			}
			while(angleNode>180) angleNode-=360;
			while(angleNode<-180) angleNode+=360;
					
		if(x < 20 && x > -20 && z < 20 &&  z > -20 && angleNode>-30 && angleNode<30){
				obstacle=true;
		}
		if(x < 10 && x > -10 && z < 10 &&  z > -10 && angleNode>-5 && angleNode<5){
				marchearriere=true;
		}
		if(x < 20 && x > -20 && z < 20 &&  z > -20 && angleNode>20 && angleNode<30){
				gauche=true;
		}
		if(x < 20 && x > -20 && z < 20 &&  z > -20 && angleNode<-20 && angleNode>-30){
				droite=true;
		}
	}
	

	if(marchearriere){
		move(-1);
	}
	else{
	//Direction le prochain noeud
	x=nodeTo->getPosition().x-pos.x;
	z=nodeTo->getPosition().z-pos.z;
	angleNode=atan(x/z)*180/M_PI-angle;
	if(z<0){ 
		if(angleNode<0) angleNode+=180;
		else angleNode-=180;
	}
	
	while(angleNode>180) angleNode-=360;
	while(angleNode<-180) angleNode+=360;
	if(obstacle && !droite){
		rotate(-1);
	}
	else if(obstacle && !gauche){
		rotate(1);
	}
	if(angleNode<7 && !droite){
		rotate(-1);
	}
	else if (angleNode>7 && !gauche){
		rotate(1);
	}
	
	if(speed > idle){
		if(angleNode > -20 && angleNode < 20 || obstacle) move(1);
		else if(angleNode > -100 && angleNode < 100) move(0);
		else move(-1);
	}
	else {
		move(1);
	 }
	}
	
	//Changement de noeud
	if(nodeTo->getPosition().x-pos.x < 5 && nodeTo->getPosition().x-pos.x > -5 && nodeTo->getPosition().z-pos.z < 5 && nodeTo->getPosition().z-pos.z > -5){
		nodeTo=nodeTo->next;
	}
	
}