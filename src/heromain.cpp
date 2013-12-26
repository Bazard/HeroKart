#include <iostream>
#include <cstdlib>
#include <cmath>
#include "VBO.hpp"
#include "VAO.hpp"
#include <vector>
#include <ctime>
#include "PlayerIA.h"
#include "Race.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "Program.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Menu.h"

#undef main

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_WIDTH = 800;
static const Uint32 WINDOW_HEIGHT = 600;
static const Uint32 WINDOW_BPP = 32;

using namespace glimac;

int main(int argc, char** argv) {


		if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
			std::cerr << "Unable to initialize SDL" << std::endl;
			return EXIT_FAILURE;
		}

		if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_OPENGL)) {
			std::cerr << "Unable to open the window and get an OpenGL context" << std::endl;
			return EXIT_FAILURE;
		}

		SDL_WM_SetCaption("OpenGL4Imacs", NULL);

		GLenum glewCode = glewInit();
		if(GLEW_OK != glewCode) {
			std::cerr << "Unable to initialize GLEW : " << glewGetErrorString(glewCode) << std::endl;
			return EXIT_FAILURE;
		}
		
	//Vecteur des Characters
	std::vector<Character*> Characters;
	
	//Vecteur des Players
	std::vector<PlayerIA*> Players;
	
	//Vecteur des Karts
	std::vector<Kart*> Karts;
	
	//Creation du championnat (plusieurs circuits)
	Race race;
	Track track1("../map/track1.map");
	Track track2("../map/track2.map");
	
	race.push_back(track1);
	race.push_back(track2);
	
	int sortie=menuPrincipal(Characters,race.getTracks());
	
	if(sortie==-1)
		return EXIT_SUCCESS;
	
	//Recuperation des Karts associés aux personnages
	Karts=KartWithChar(Characters);
	//Remplissage de Players
	for (int i=0;i<8;++i){
		PlayerIA* play=new PlayerIA("Player",*Karts[i],*Characters[i]);
		Players.push_back(play);
	}
	//Creation du jeu
	Game game(race,Players,Karts);
	game.playChampionShip();
	
	
	SDL_Quit();
	return EXIT_SUCCESS;
}

