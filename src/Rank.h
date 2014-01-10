#pragma once
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "Kart.h"
#include "PlayerIA.h"

void ranking(std::vector<Kart*>& karts); // Gère le classement de la course
void getFinalRanking(std::vector<PlayerIA*>& players, std::vector<SDL_Surface*> textSurfaces, TTF_Font* font); // Affiche le classement à la fin de la course
