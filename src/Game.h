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
	std::vector<Object3D*> mapObjects;
public:
	Game(Race& race, std::vector<PlayerIA*>&,std::vector<Kart*>&,std::vector<Object3D*>&);
	~Game();
	int playChampionShip();
	int playTrack(Track& track);
	void Clean(std::vector<Object3D*> objs, std::vector<PlayerIA*> players);
};

