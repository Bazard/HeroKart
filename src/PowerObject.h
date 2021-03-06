#pragma once
#include "Object3D.h"
#include "Kart.h"
#include <vector>

enum typeEnum { BOOST=0, ATK_FRONT=1, ATK_BACK=2, ATK_ALL=3, SHIELD=4, TRAP=5 };

class PowerObject: public Object3D
{
private:
	typeEnum type;
	
	bool launched;  // indique son etat (lancé ou pas)
	int duration; //sa durée
	int timeOfUse; //le moment quand le pouvoir est lancé
	std::vector<float> stock; //stockage de donnée du kart si besoin
	bool pick; //Est-ce que l'objet est ramassable
public:
	PowerObject(typeEnum type, int duration);
	PowerObject(const PowerObject& obj);
	~PowerObject();
	int getDuration(){return duration;}
	int getTimeOfUse(){return timeOfUse;}
	void setTimeOfUse(int t){timeOfUse=t;}
	bool isLaunched() {return launched;}
	int power(std::vector<Kart*>& vecKart, int idLanceur,int tStart, std::vector<Object3D*>& objs);
	void powerBack(std::vector<Kart*>& vecKart, int idLanceur);
	bool withKart();
	void movePower();
	bool tooFar(); //Teste si l'objet est trop de loin de la scène
	bool isPerimed(int tStart);
	int hitKart(Kart& kart,int id,int tStart);
	void hitKartBack(std::vector<Kart*>& karts);
	typeEnum getType(){ return type;}
	void setPick(bool b){ pick=b;}
};

