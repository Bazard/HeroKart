#pragma once
#include "Race.h"
#include "PlayerIA.h"
#include <vector>
#include "Kart.h"

class Game
{
private:
	Race race;
	std::vector<PlayerIA*> Players;
	std::vector<Kart*> Karts;

public:
	Game(Race& race, std::vector<PlayerIA*>&,std::vector<Kart*>&);
	~Game();
	
	int playChampionShip(); //Joue tout le championnat
	int playTrack(Track& track); //Joue une course du championnat
	
	void CleanObjects(Track& track); //Nettoie les objets de la map qui vient d'être utilisé
	void CleanAll(); //Nettoie tout le reste (Perso, Kart)
};

