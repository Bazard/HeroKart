#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstdlib>
#include "Character.h"
#include "Race.h"

 static Uint32 WINDOW_WIDTH=800;
 static Uint32 WINDOW_HEIGHT=600;

int menuCircuit(std::vector<Character*>&,std::vector<Track*>&);
int menuPersonnage(std::vector<Character*>&,std::vector<Track*>&);
int lancerJeuRandom();
int menuOptions(std::vector<Character*>&,std::vector<Track*>&);
int menuPrincipal(std::vector<Character*>&,std::vector<Track*>&);
std::vector<Kart*>& KartWithChar(std::vector<Character*>&);
void restartSDL();