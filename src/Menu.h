#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstdlib>
#include "Character.h"
#include "Race.h"

int menuCircuit(std::vector<Character*>&,std::vector<Track*>&);
int menuPersonnage(std::vector<Character*>&,std::vector<Track*>&);
void lancerJeuRandom();
int menuPrincipal(std::vector<Character*>&,std::vector<Track*>&);
std::vector<Kart*>& KartWithChar(std::vector<Character*>&);