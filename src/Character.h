#pragma once
#include "Kart.h"

enum Hero { JOHN=0, KLAUS=1, DOUG=2, CANADA=3, BURT=4, MCKORMACK=5, STEVE=6, VALUR=7 };

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
public:
	Character(Hero,int);
	~Character();
	
	bool isPowerReady(int tStart);
	void useSuperPower(int tStart, Kart& kart);
	void useSuperPowerBack(Kart& kart);
	bool isPerimed(int tStart);
	bool isLaunched(){return launched;}
	void hitSuperPower(int tStart,Kart& kart);
	void hitSuperPowerBack(Kart& kart);
};

