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

void Clean(std::vector<Object3D*> objs, std::vector<PlayerIA*> players);

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
	
	Kart kart2(2,0.01,0.75,5);
	kart2.setPosition(glm::vec3(10,0.5,-10));
	kart2.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart2.buildVBO();
	kart2.buildVAO();
	kart2.LoadTexture("../textures/EarthMap.jpg");
	
	Kart kart3(2,0.01,0.75,5);
	kart3.setPosition(glm::vec3(10,0.5,10));
	kart3.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart3.buildVBO();
	kart3.buildVAO();
	kart3.LoadTexture("../textures/MoonMap.png");
	
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
	Karts.push_back(&kart2);
	Karts.push_back(&kart3);
	
	mapObjects.push_back(&floor);
	
	PowerObject boost(BOOST,10000);
	PowerObject atk_back(ATK_BACK,10000);
	PowerObject trap(TRAP,10000);
	PowerObject shield(SHIELD, 10000);
	PowerObject atk_all(ATK_ALL, 10000);
	PowerObject atk_front(ATK_FRONT, 10000);
	
	PowerObject *obj;
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
			if(brubru.getPower()->isLaunched() && brubru.getPower()->withKart()){
				brubru.getPower()->getVAO().bind();		//Bind du VAO
				brubru.getPower()->TransfoMatrix(ViewMatrix, kart.getPosition()); //Transformations (View, Translation, anglerotation,scale)
				brubru.getPower()->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
				brubru.getPower()->Draw(uTex);	//Draw de l'objet
			}
			
			if(brubru.getPower()->isLaunched() && brubru.getPower()->isPerimed(tStart)){
				brubru.stopPower(Karts,0);
			}
		}
		
		//Kart (boucle sur tous les karts)
		for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
			(*it)->getVAO().bind();		//Bind du VAO
			(*it)->TransfoMatrix(ViewMatrix,(*it)->getPosition()); //Transformations (View, Translation, anglerotation,scale)
			(*it)->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
			(*it)->Draw(uTex);	//Draw de l'objet
		}
		
		//Dessin des objets de la map
		for (std::vector<Object3D*>::iterator it = mapObjects.begin() ; it != mapObjects.end(); ++it){
			if((*it)->isVisible()){
				(*it)->getVAO().bind();		
				(*it)->movePower();
				(*it)->TransfoMatrix(ViewMatrix, (*it)->getPosition());
				(*it)->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
				(*it)->Draw(uTex);
				
				if((*it)->tooFar()){
					mapObjects.erase(it);	
					delete(*it);
					it--;
				}
				
				else if((*it)->isPerimed(tStart)){
					(*it)->hitKartBack(Karts);
					mapObjects.erase(it);	
					delete(*it);
					it--;
				}
			}
		}
		
		//Sky
		sky.getVAO().bind();		
		sky.setAngle(tStart*0.001f);
		sky.TransfoMatrix(ViewMatrix,sky.getPosition());
		sky.MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
		sky.Draw(uTex);
		
		VAO::debind();
		
		//Teste collision Kart/Objets
		
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {			
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case 'n':
							std::cout << "You pick an Attack_Front" << std::endl;
							brubru.pickPower(atk_front);
							obj=brubru.getPower();
							break;
						case 'b':
							std::cout << "You pick a Boost" << std::endl;
							brubru.pickPower(boost);
							break;
						case 'v':
							std::cout << "You pick an Attack_back" << std::endl;
							brubru.pickPower(atk_back);
							obj=brubru.getPower();
							break;
						case 'c':
							std::cout << "You pick a Trap" << std::endl;
							brubru.pickPower(trap);
							obj=brubru.getPower();
							break;
						case 'x':
							std::cout << "You pick a Shield" << std::endl;
							brubru.pickPower(shield);
							break;
						case 'z':
							std::cout << "You pick an Attack_All" << std::endl;
							brubru.pickPower(atk_all);
							break;
						case 'm':
							std::cout << "You have been hit" << std::endl;
							obj->hitKart(kart, 0, tStart);
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
	
	Clean(mapObjects,Players);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void Clean(std::vector<Object3D*> objs, std::vector<PlayerIA*> players){
	for (std::vector<Object3D*>::iterator it = objs.begin() ; it != objs.end(); ++it){
		delete(*it);
		it--;
	}
	
	for (std::vector<PlayerIA*>::iterator it = players.begin() ; it != players.end(); ++it){
		delete(&(*it)->getKart());
		delete(&(*it)->getCharacter());
		delete(*it);
		it--;
	}
}