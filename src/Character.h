#pragma once
class Character
{
private:
	int idCharacter;
	char* characterName;
	//Power power;
	int timeOfUse;
	int reloadtime;
public:
	Character();
	~Character();
	
	bool isPowerReady(int tStart);
	void useSuperPower(int tStart);
};

