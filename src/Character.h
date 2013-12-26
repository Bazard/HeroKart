#pragma once
#include "Kart.h"
#include <vector>

class Object3D;

enum Hero { JOHN=0, KLAUS=1, DOUG=2, CANADA=3, BURT=4, MCKORMACK=5, STEVE=6, STAN=7, JENNIFER=8 };

class Character
{
private:
	Hero hero;
	char* characterName;
	//Power power;
	int timeOfUse;
	int reloadtime;
	int duration;
	bool launched;
	float stock; //Stockage de données
public:
	Character(Hero,int);
	~Character();
	
	bool isPowerReady(int tStart);
	void useSuperPower(int tStart, Kart& kart, std::vector<Object3D*>& mapObjects); //Effet lorsque l'on active le pouvoir
	void useSuperPowerBack(Kart& kart); //Effet retour lorsque le pouvoir est epuisé
	bool isPerimed(int tStart); //Est-ce que le pouvoir est epuisé ?
	bool isLaunched(){return launched;} //Est-ce que le pouvoir est activé ?
	void hitSuperPower(int tStart,Kart& kart); //Effet lorsque l'on touche un adversaire avec le pouvoir. Une attaque physique
	void hitSuperPowerBack(Kart& kart); //Effet retour
	// void continuousHitSuperPower(Kart& kartHit, Kart& KartFrom); //Effet continue
	
	Hero getHero(){ return hero;}
};

