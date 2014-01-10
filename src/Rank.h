#pragma once
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include "Kart.h"
#include "PlayerIA.h"

void ranking(std::vector<Kart*>& karts); // Gère le classement de la course
void getFinalRanking(std::vector<PlayerIA*>& players, std::vector<SDL_Surface*> textSurfaces, TTF_Font* font); // Affiche le classement à la fin de la course

std::vector<SDL_Surface*> createRankSurfaces(TTF_Font* font);
std::vector<SDL_Surface*> createCharactersSurfaces();
std::vector<SDL_Rect> createPositions();

void showRankSurfaces(std::vector<SDL_Surface*> surfaces, SDL_Surface* screen, std::vector<SDL_Rect> positions);