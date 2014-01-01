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
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include "Menu.h"

#undef main

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_BPP = 32;

using namespace glimac;

int main(int argc, char** argv) {

		// Initialisation de la SDL
		if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
			std::cerr << "Unable to initialize SDL" << std::endl;
			return EXIT_FAILURE;
		}

		if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_OPENGL)) {
			std::cerr << "Unable to open the window and get an OpenGL context" << std::endl;
			return EXIT_FAILURE;
		}

		SDL_WM_SetCaption("OpenGL4Imacs", NULL);

		// Initialisation de OpenGL
		GLenum glewCode = glewInit();
		if(GLEW_OK != glewCode) {
			std::cerr << "Unable to initialize GLEW : " << glewGetErrorString(glewCode) << std::endl;
			return EXIT_FAILURE;
		}

		// Initialisation de SDL_Mixer
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
	      std::cout << Mix_GetError() << std::endl;
	    }

	    Mix_Music *musique; //Création du pointeur de type Mix_Music
		musique = Mix_LoadMUS("../sounds/musique3.mp3"); //Chargement de la musique
		Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
		
	//Vecteur des Characters
	std::vector<Character*> Characters;
	
	//Vecteur des Players
	std::vector<PlayerIA*> Players;
	
	//Vecteur des Karts
	std::vector<Kart*> Karts;
	
	//Creation du championnat (plusieurs circuits)
	Race race;
	
	int sortie=redirection(Characters,race.getTracks());
	
	if(sortie==-1){
		std::cout << "Sortie" << std::endl;
		return EXIT_SUCCESS;
	}

	//Recuperation des Karts associés aux personnages
	KartWithChar(Characters,Karts);
	//Remplissage de Players
	for (int i=0;i<8;++i){
		PlayerIA* play=new PlayerIA("Player",*Karts[i],*Characters[i]);
		Players.push_back(play);
	}
	
	//Creation du jeu
	Game game(race,Players,Karts);
	game.playChampionShip();
	
	Mix_FreeMusic(musique); //Libération de la musique
    Mix_CloseAudio(); //Fermeture de l'API

	SDL_Quit();
	return EXIT_SUCCESS;
}

