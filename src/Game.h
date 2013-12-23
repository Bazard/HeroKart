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
	int playChampionShip();
	int playTrack(Track& track);
	void CleanObjects(Track& track);
	void CleanAll();
};

