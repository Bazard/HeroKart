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
		
	
	
	//Creation des karts
	Kart kart(2,0.01,0.75,5);
	kart.setPosition(glm::vec3(0,0.5,0));
	kart.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart.LoadTexture("../textures/CCTex.jpg");
	kart.build();

	Kart kart2(2,0.01,0.75,5);
	kart2.setPosition(glm::vec3(10,0.5,-10));
	kart2.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart2.build();
	kart2.LoadTexture("../textures/EarthMap.jpg");
	
	Kart kart3(2,0.01,0.75,5);
	kart3.setPosition(glm::vec3(10,0.5,10));
	kart3.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart3.build();
	kart3.LoadTexture("../textures/MoonMap.png");

	Kart kart4(2,0.01,0.75,5);
	kart4.setPosition(glm::vec3(10,0.5,10));
	kart4.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart4.build();
	kart4.LoadTexture("../textures/MoonMap.png");
	
	Kart kart5(2,0.01,0.75,5);
	kart5.setPosition(glm::vec3(10,0.5,10));
	kart5.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart5.build();
	kart5.LoadTexture("../textures/MoonMap.png");
	
	Kart kart6(2,0.01,0.75,5);
	kart6.setPosition(glm::vec3(10,0.5,10));
	kart6.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart6.build();
	kart6.LoadTexture("../textures/MoonMap.png");
	
	Kart kart7(2,0.01,0.75,5);
	kart7.setPosition(glm::vec3(10,0.5,10));
	kart7.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart7.build();
	kart7.LoadTexture("../textures/MoonMap.png");
	
	Kart kart8(2,0.01,0.75,5);
	kart8.setPosition(glm::vec3(10,0.5,10));
	kart8.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart8.build();
	kart8.LoadTexture("../textures/MoonMap.png");
	
	//Vecteur des Characters
	std::vector<Character*> Characters;
	
	//Vecteur des Players
	std::vector<PlayerIA*> Players;
	
	//Vecteur des Karts
	std::vector<Kart*> Karts;
	
	Karts.push_back(&kart);
	Karts.push_back(&kart2);
	Karts.push_back(&kart3);
	Karts.push_back(&kart4);
	Karts.push_back(&kart5);
	Karts.push_back(&kart6);
	Karts.push_back(&kart7);
	Karts.push_back(&kart8);
	
	//La boucle du menu doit commencer ici ! Et il faut créer les joueurs en fonction des choix de l'utilisateur.
	// L'utilisateur doit etre le premier joueur dans le vecteur
	
	//Creation du championnat (plusieurs circuits)
	Race race;
	Track track1("../map/track1.map");
	Track track2("../map/track2.map");
	
	race.push_back(track1);
	race.push_back(track2);
	
	int sortie=menuPrincipal(Characters,Karts,race.getTracks());
	
	for (int i=0;i<8;++i){
		PlayerIA* play=new PlayerIA("Player",*Karts[i],*Characters[i]);
		Players.push_back(play);
	}
	//Creation du jeu
	if(sortie!=-1){
		Game game(race,Players,Karts);
		game.playChampionShip();
	}
	
	SDL_Quit();
	return EXIT_SUCCESS;
}

