#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstdlib>
#include "Character.h"
#include "Race.h"

static Uint32 WINDOW_WIDTH=800;
static Uint32 WINDOW_HEIGHT=600;

int redirection(std::vector<Character*>&,std::vector<Track*>&);
void menuChargement();
int menuCircuit(std::vector<Character*>&,std::vector<Track*>&);
int menuPersonnage(std::vector<Character*>&,std::vector<Track*>&);
int lancerJeuRandom();
int menuOptions(std::vector<Character*>&,std::vector<Track*>&);
int menuPrincipal(std::vector<Character*>&,std::vector<Track*>&);
void KartWithChar(std::vector<Character*>&,std::vector<Kart*>&);

void restartSDL();