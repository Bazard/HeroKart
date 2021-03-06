#pragma once
#include "Kart.h"
#include <vector>
#include <cstring>

class Object3D;

// enum Hero { JOHN=0, KLAUS=1, DOUG=2, CANADA=3, BURT=4, MCKORMACK=5, STEVE=6, STAN=7, JENNIFER=8 };

class Character
{
private:
	Hero hero;
	const char* characterName;
	//Power power;
	int timeOfUse;
	int reloadtime;
	int duration;
	bool launched;
	std::vector<float> stock; //Stockage de donn�es
public:
	Character(Hero,int, const char*);
	~Character();
	
	bool isPowerReady(int tStart);
	void useSuperPower(int tStart, Kart& kart, std::vector<Object3D*>& mapObjects); //Effet lorsque l'on active le pouvoir
	void useSuperPowerBack(Kart& kart); //Effet retour lorsque le pouvoir est epuis�
	bool isPerimed(int tStart); //Est-ce que le pouvoir est epuis� ?
	bool isLaunched(){return launched;} //Est-ce que le pouvoir est activ� ?
	void hitSuperPower(int tStart,std::vector<Kart*>& karts, int idTouche, Kart& kartFrom);//Effet lorsque l'on touche un adversaire avec le pouvoir. Une attaque physique
	void hitSuperPowerBack(std::vector<Kart*>& karts); //Effet retour
	void continuousHitSuperPower(std::vector<Kart*>& karts, Kart& kart); //Effet continue

	const char* getCharacterName(){ return characterName; };
	
	Hero getHero(){ return hero;}
};

