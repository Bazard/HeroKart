#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstdlib>
#include "PlayerIA.h"
#include "Race.h"
#include "Vertex2DUV.hpp"
#include <SOIL/SOIL.h>
#include <SDL/SDL_ttf.h>

static Uint32 WINDOW_WIDTH=800;
static Uint32 WINDOW_HEIGHT=600;

int redirection(std::vector<Character*>& characters,std::vector<Track*>& tracks);
int redirectionPause();
int menuPause();
void menuChargement();
int menuCircuit(std::vector<Character*>&,std::vector<Track*>&);
int menuPersonnage(std::vector<Character*>&,std::vector<Track*>&);
int lancerJeuRandom(std::vector<Character*>&,std::vector<Track*>&);
int menuOptions();
int menuPrincipal(std::vector<Character*>&,std::vector<Track*>&);
void KartWithChar(std::vector<Character*>&,std::vector<Kart*>&);
void restartSDL();
GLuint* PowerTexture();
GLuint* RankTexture();
GLuint* PersoTexture();
GLuint creerTextureTexte(const char* texte, int largeurTexte, int hauteurTexte, unsigned int taille, const char* policeTexte, Uint8 r, Uint8 g, Uint8 b);