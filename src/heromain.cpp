#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "VBO.hpp"
#include "VAO.hpp"
#include "Vertex3DRGB.hpp"
#include "Program.hpp"
#include <vector>
#include <ctime>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "common.hpp"
#include <glm/gtc/random.hpp>
#include "TrackballCamera.hpp"
#include "FreeflyCamera.hpp"
#include "PlayerIA.h"
#include <queue>
// #include <SDL/SDL_ttf.h>
// #include "Menu.h"

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

	// if(TTF_Init() == -1)
	// {
		// std::cout << "Erreur de TTF" << std::endl;
	// }
			
	//kart
	Kart kart(2,0.01,0.75,5);
	kart.setPosition(glm::vec3(0,0.5,0));
	kart.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart.buildVBO();
	kart.buildVAO();
	kart.LoadTexture("../textures/CCTex.jpg");
	
	//Objet2
	Object3D floor;
	floor.LoadObjFromFile("../models/floor.obj");	
	floor.setScale(glm::vec3(16,1,16));
	floor.build();
	floor.LoadTexture("../textures/MoonMap.png");

	//Ciel
	Object3D sky;
	sky.sphere(1,32,16);
	sky.setScale(glm::vec3(30,30,30));
	sky.build();
	sky.LoadTexture("../textures/sky.jpg");
	
	Program prog;
	if(argc == 3){
		prog=loadProgram(argv[1],argv[2]);
		prog.use();
		}
	else{
		prog=loadProgram("../shaders/3D.vs.glsl","../shaders/tex3D.fs.glsl");
		prog.use();
	}
	
	
	GLuint uMVPMatrix=glGetUniformLocation(prog.getGLId(),"uMVPMatrix");
	GLuint uMVMatrix=glGetUniformLocation(prog.getGLId(),"uMVMatrix");
	GLuint uNormalMatrix=glGetUniformLocation(prog.getGLId(),"uNormalMatrix");
	GLuint uTex=glGetUniformLocation(prog.getGLId(),"uTexture");
	
	glEnable(GL_DEPTH_TEST);
	
	glm::mat4 ViewMatrix;
		
	TrackballCamera camera;
	 
	 //Rotation pour la caméra
	std::queue<std::pair<float,Uint32>> anglefile;
	anglefile.push(std::pair<float,Uint32>(0,0));
	
	//Vecteurs Players et Karts
	std::vector<PlayerIA*> Players;
	std::vector<Kart*> Karts;
	
	//Vecteurs des objets de la map
	std::vector<Object3D*> mapObjects;
	
	Character SuperBru;
	PlayerIA brubru("Brubru", kart, SuperBru);
	
	Players.push_back(&brubru);
	Karts.push_back(&kart);
	
	mapObjects.push_back(&floor);
	
	PowerObject boost(BOOST,10000);
	PowerObject atk_back(ATK_BACK,0);
	PowerObject trap(TRAP,0);
	bool done=false;
	while(!done) {

		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Camera
		
		if(tStart-anglefile.front().second>330){
			if(anglefile.size()>1)
				anglefile.pop();
		}
		
		ViewMatrix=camera.getViewMatrix(kart.getPosition(), anglefile.front().first,kart.back);
	
		//Pouvoirs (boucle sur tous les persos à faire)
		if(brubru.getPower()){
				
			if(brubru.getPower()->isLaunched() && brubru.getPower()->getDuration()+brubru.getPower()->getTimeOfUse() < tStart){
				brubru.stopPower(Karts,0);
			}
		}
		
		//Kart (boucle sur tous les karts à faire)
		kart.getVAO().bind();		//Bind du VAO
		kart.TransfoMatrix(ViewMatrix, kart.getAngle()); //Transformations (View, Translation, anglerotation,scale)
		kart.MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
		kart.Draw(uTex);	//Draw de l'objet

		//Dessin des objets de la map
		for (std::vector<Object3D*>::iterator it = mapObjects.begin() ; it != mapObjects.end(); ++it){
			if((*it)->isVisible()){
				(*it)->getVAO().bind();		
				(*it)->TransfoMatrix(ViewMatrix, 0);
				(*it)->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
				(*it)->Draw(uTex);
				
			}
		}
		
		//Sky
		sky.getVAO().bind();		
		sky.TransfoMatrix(ViewMatrix, tStart* 0.001f);
		sky.MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
		sky.Draw(uTex);
		
		VAO::debind();
		
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {			
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case 'b':
							std::cout << "You pick a Boost" << std::endl;
							brubru.pickPower(boost);
							break;
						case 'v':
							std::cout << "You pick an Attack_back" << std::endl;
							brubru.pickPower(atk_back);
							break;
						case 't':
							std::cout << "You pick a Trap" << std::endl;
							brubru.pickPower(trap);
							break;
						case SDLK_SPACE:
							brubru.usePower(Karts,0,tStart,mapObjects);
							
							break;
					}
					break;
				default:
					break;
				
			}
		}
		
		Uint8 *keystate = SDL_GetKeyState(NULL);
	
		if ( keystate[SDLK_UP] ) kart.move(1);
		if ( keystate[SDLK_DOWN] ) kart.move(-1);
		if (!keystate[SDLK_UP] && !keystate[SDLK_UP]) kart.move(0);
		
		if ( keystate[SDLK_LEFT] ){ kart.rotate(1); anglefile.push(std::pair<float,Uint32>(kart.getAngle(),tStart));}
		if ( keystate[SDLK_RIGHT] ){ kart.rotate(-1); anglefile.push(std::pair<float,Uint32>(kart.getAngle(),tStart));}
		
		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if(d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}
	
	SDL_Quit();

	return EXIT_SUCCESS;
}