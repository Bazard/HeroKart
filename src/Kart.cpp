#include "Kart.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include "PowerObject.h"

#define EPSILON 0.00001

Kart::Kart():tourne(2.f), acceleration(0.1f), speed(0),speedmax(0.5), back(false), poids(5),invincible(false),intouchable(false),nodeTo(NULL),nbNodesPassed(0),rank(0),idTexture2(-1){
	dir.z=1;
}

Kart::Kart(float tourne, float acceleration, float speedmax, float poids) :tourne(tourne), acceleration(acceleration), speedmax(speedmax), speed(0), back(false), poids(poids),invincible(false),intouchable(false),nodeTo(NULL),nbNodesPassed(0),rank(0),idTexture2(-1)
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





void Kart::incrRank(){
	if(rank < 8){ rank++; }
	else{  }
}

void Kart::decrRank(){
	if(rank > 1){ rank--; }
	else{  }
}


void Kart::crossANode(void){
	glm::vec3 u;
	u.x = nodeTo->getRightPos().x - nodeTo->getLeftPos().x;
	u.y = 0;
	u.z = nodeTo->getRightPos().z - nodeTo->getLeftPos().z;

	glm::vec3 AC;
	AC.x = pos.x - nodeTo->getLeftPos().x;
	AC.y = 0;
	AC.z = pos.z - nodeTo->getLeftPos().z;

	float numerateur = u.x*AC.z - u.z*AC.x;   // norme du vecteur v
	if (numerateur < 0)
		numerateur = -numerateur ;   // valeur absolue ; si c'est négatif, on prend l'opposé.
	float denominateur = sqrt(u.x*u.x + u.z*u.z);  // norme de u
	float CI = numerateur / denominateur;
	
	// Si on franchit le checkpoint
	if (CI<2*hitbox.x){
		nbNodesPassed++;
		nodeTo=nodeTo->next;
	}
}


float Kart::distanceToNextNode(void){
	return sqrt( pow((nodeTo->getPosition().x - pos.x),2) + pow((nodeTo->getPosition().y - pos.y),2) + pow((nodeTo->getPosition().z - pos.z),2) );
}





int Kart::moveIA(std::vector<Object3D*>& mapObjects,std::vector<PowerObject*>& powObjects, std::vector<Kart*>& Karts, PowerObject* power, Hero hero, bool powerReady){
	float idle=0.05;
	float angleNode,x,z;
	bool marchearriere=false, droite=false, gauche=false, obstacle=false;
	int sortie=0;
			
	//Repere les obstacles
	for (std::vector<Object3D*>::iterator it = mapObjects.begin()+1 ; it != mapObjects.end(); ++it){
		if(!(*it)->isVisible())
			continue;
			
		x=(*it)->getPosition().x-pos.x;
		z=(*it)->getPosition().z-pos.z;		
		angleNode=getAngle2Vec(x,z);
			
		if(x <= 20 && x >= -20 && z <= 20 &&  z >= -20 && angleNode>=-30 && angleNode<=30){
				obstacle=true;
			if(x <= 10 && x >= -10 && z <= 10 &&  z >= -10 && angleNode>=-5 && angleNode<=5){
					marchearriere=true;
			}
			if(x <= 20 && x >= -20 && z <= 20 &&  z >= -20 && angleNode>=20 && angleNode<=30){
					gauche=true;
			}
			if(x <= 20 && x >= -20 && z <= 20 &&  z >= -20 && angleNode<=-20 && angleNode>=-30){
					droite=true;
			}
		}
	}
		//Utilise le super pouvoir du personnage
	if(!obstacle && powerReady){
		for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
			x=(*it)->getPosition().x-pos.x;
			z=(*it)->getPosition().z-pos.z;		
			angleNode=getAngle2Vec(x,z);
			
			
			if(hero==STEVE){
				if(x <= 30 && x >= -30 && z <= 30 &&  z >= -30 && angleNode<=5 && angleNode>=-5){
						sortie=2;
				}
			}
			else if(hero==BURT){
				if(x <= 30 && x >= -30 && z <= 30 &&  z >= -30 && (angleNode<=-175 || angleNode>=175)){
						// sortie=2;
				}
			}
			else if(hero == KLAUS || hero == DOUG || hero == STAN){
				if(x <= 6 && x >= -6 && z <= 6 &&  z >= -6){
					sortie=2;
				}
			}
			else{
				sortie=2;
			}
		}
	}
	else if(!obstacle && !power){
		//Cherche un pouvoir sur la route
		for (std::vector<PowerObject*>::iterator it = powObjects.begin() ; it != powObjects.end(); ++it){
			if(!(*it)->isVisible())
				continue;
				
			x=(*it)->getPosition().x-pos.x;
			z=(*it)->getPosition().z-pos.z;		
			angleNode=getAngle2Vec(x,z);
			
			if(x <= 40 && x >= -40 && z <= 40 &&  z >= -40 && angleNode>=-20 && angleNode<=20){
					obstacle=true;
					if(this==Karts[0])
						std::cout << "Pouvoirs detected" << std::endl;
			}
			if(x <= 40 && x >= -40 && z <= 40 &&  z >= -40 && angleNode>=10 && angleNode<=20){
					droite=true;
					gauche=false;
			}
			if(x <= 40 && x >= -40 && z <= 40 &&  z >= -40 && angleNode<=-10 && angleNode>=-20){
					gauche=true;
					droite=false;
			}
		}
	}
	//S'il a déjà un pouvoir, il va chercher à piner quelqu'un
	else if(!obstacle && power){
		for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
			if((*it)==this)
				continue;
				
			x=(*it)->getPosition().x-pos.x;
			z=(*it)->getPosition().z-pos.z;		
			angleNode=getAngle2Vec(x,z);
			
			//Cherche devant
			if(power->getType() == ATK_FRONT){
				if(x <= 30 && x >= -30 && z <= 30 &&  z >= -30 && angleNode<=5 && angleNode>=-5){
						sortie=1;
				}
			}
			//Cherche derriere
			else if(power->getType() == ATK_BACK || power->getType()==TRAP){
				if(x <= 30 && x >= -30 && z <= 30 &&  z >= -30 && (angleNode<=-175 || angleNode>=175)){
						sortie=1;
				}
			}
			else {
				sortie=1;
			}
		}
	}

	//Marche arriere si obstacle
	if(marchearriere){
		move(-1);
	}
	else{
	//Direction le prochain noeud
	angleNode=getAngle2Vec(nodeTo->getPosition(),pos);
	
	if(obstacle && !droite){
		rotate(-1);
	}
	else if(obstacle && !gauche){
		rotate(1);
	}
	if(angleNode<=-7 && !droite){
		rotate(-1);
	}
	else if (angleNode>=7 && !gauche){
		rotate(1);
	}
	
	if(speed > idle){
		if(angleNode >= -10 && angleNode <= 10 || obstacle){
		move(1); //J'Accelere !
		}
		else if(angleNode >= -50 && angleNode <= 50){ move(0);}//Lache l'accelerateur
		else {move(-1);} //Freine
	}
	else {
		move(1);
	 }
	}
	
	//Changement de noeud
	/*if(nodeTo->getPosition().x-pos.x < 5 && nodeTo->getPosition().x-pos.x > -5 && nodeTo->getPosition().z-pos.z < 5 && nodeTo->getPosition().z-pos.z > -5){
		nodeTo=nodeTo->next;
	}*/
	crossANode();
	
	return sortie;
}

float Kart::getAngle2Vec(glm::vec3 vec2, glm::vec3 vec1){
			float x=vec2.x-vec1.x;
			float z=vec2.z-vec1.z;
			
			float angleNode=atan(x/z)*180/M_PI-angle;
			
			if(z<=0){ 
					if(angleNode<0) angleNode+=180;
					else angleNode-=180;
				}
				while(angleNode>180) angleNode-=360;
				while(angleNode<-180) angleNode+=360;
				
			return angleNode;
}

float Kart::getAngle2Vec(float x, float z){
			
			float angleNode=atan(x/z)*180/M_PI-angle;
			
			if(z<0){ 
					if(angleNode<0) angleNode+=180;
					else angleNode-=180;
				}
				while(angleNode>180) angleNode-=360;
				while(angleNode<-180) angleNode+=360;
				
			return angleNode;
}