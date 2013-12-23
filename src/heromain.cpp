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
		
	// Program prog;
	// prog=loadProgram("../shaders/3D.vs.glsl","../shaders/tex3D.fs.glsl");
	// prog.use();
	
	//Creation des karts
	Kart kart(2,0.01,0.75,5);
	kart.setPosition(glm::vec3(0,0.5,0));
	kart.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart.build();
	kart.LoadTexture("../textures/CCTex.jpg");

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

	//Objet2
	Object3D floor;
	floor.LoadObjFromFile("../models/floor.obj");	
	floor.setScale(glm::vec3(16,1,16));
	floor.build();
	floor.LoadTexture("../textures/MoonMap.png");


	//Vecteur des Players
	std::vector<PlayerIA*> Players;
	
	//Vecteur des Karts
	std::vector<Kart*> Karts;
	
	//Vecteur des objets de la map
	std::vector<Object3D*> mapObjects;
	
	//La boucle du menu doit commencer ici ! Et il faut créer les joueurs en fonction des choix de l'utilisateur.
	// L'utilisateur doit etre le premier joueur dans le vecteur
	
	//Creation des personnages
	Character SuperBru;
	Character CaptainCyriuk;
	Character SuperCali;
	
	//Creation des joueurs
	PlayerIA brubru("Brubru", kart, SuperBru);
	PlayerIA cyril("Cyril", kart2, CaptainCyriuk);
	PlayerIA marie("Marie", kart3, SuperCali);
	
	//On les met dans les vecteurs (dans l'ORDRE !)
	Players.push_back(&brubru);
	Karts.push_back(&kart);
	
	Players.push_back(&cyril);
	Karts.push_back(&kart2);
	
	Players.push_back(&marie);
	Karts.push_back(&kart3);
	
	mapObjects.push_back(&floor);
	
	//Creation du championnat (plusieurs circuits)
	Race race;
	Track track1;
	
	race.push_back(track1);
	//Creation du jeu
	Game game(race,Players,Karts,mapObjects);
	
	//Et on lance la partie
	game.playChampionShip();
	
	
	SDL_Quit();
	return EXIT_SUCCESS;
}

