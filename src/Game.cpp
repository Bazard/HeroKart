#include "Game.h"
#include "Program.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "TrackballCamera.hpp"
#include <iostream>
#include "VAO.hpp"
#include <queue>
#include <cstdlib>

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_WIDTH = 800;
static const Uint32 WINDOW_HEIGHT = 600;
static const Uint32 WINDOW_BPP = 32;

using namespace glimac;

Game::Game(Race & race, std::vector<PlayerIA*>& players,std::vector<Kart*>& karts,std::vector<Object3D*>& mapObjects):race(race),Players(players), Karts(karts), mapObjects(mapObjects)
{
}


Game::~Game()
{
}

int Game::play(){

		Program prog;
		prog=loadProgram("../shaders/3D.vs.glsl","../shaders/tex3D.fs.glsl");
		prog.use();
	
	
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
	
	PowerObject boost(BOOST,10000);
	PowerObject atk_back(ATK_BACK,10000);
	PowerObject trap(TRAP,10000);
	PowerObject shield(SHIELD, 10000);
	PowerObject atk_all(ATK_ALL, 10000);
	PowerObject atk_front(ATK_FRONT, 10000);
	
	//Ciel
	Object3D sky;
	sky.sphere(1,32,16);
	sky.setScale(glm::vec3(30,30,30));
	sky.build();
	sky.LoadTexture("../textures/sky.jpg");
	
	PowerObject *obj=NULL;
	bool done=false;
	while(!done) {

		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Camera
		
		if(tStart-anglefile.front().second>330){
			if(anglefile.size()>1)
				anglefile.pop();
		}
		
		ViewMatrix=camera.getViewMatrix(Karts[0]->getPosition(), anglefile.front().first,Karts[0]->back);
	
		//Pouvoirs (boucle sur tous les persos)
		for (std::vector<PlayerIA*>::iterator it = Players.begin() ; it != Players.end(); ++it){
			if((*it)->getPower()){
				if((*it)->getPower()->isLaunched() && (*it)->getPower()->withKart()){
					(*it)->getPower()->getVAO().bind();		//Bind du VAO
					(*it)->getPower()->TransfoMatrix(ViewMatrix, (*it)->getKart().getPosition()); //Transformations (View, Translation, anglerotation,scale)
					(*it)->getPower()->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
					(*it)->getPower()->Draw(uTex);	//Draw de l'objet
				}
				
				if((*it)->getPower()->isLaunched() && (*it)->getPower()->isPerimed(tStart)){
					(*it)->stopPower(Karts,0);
				}
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
							Players[0]->pickPower(atk_front);
							obj=Players[0]->getPower();
							break;
						case 'b':
							std::cout << "You pick a Boost" << std::endl;
							Players[0]->pickPower(boost);
							break;
						case 'v':
							std::cout << "You pick an Attack_back" << std::endl;
							Players[0]->pickPower(atk_back);
							obj=Players[0]->getPower();
							break;
						case 'c':
							std::cout << "You pick a Trap" << std::endl;
							Players[0]->pickPower(trap);
							obj=Players[0]->getPower();
							break;
						case 'x':
							std::cout << "You pick a Shield" << std::endl;
							Players[0]->pickPower(shield);
							break;
						case 'z':
							std::cout << "You pick an Attack_All" << std::endl;
							Players[0]->pickPower(atk_all);
							break;
						case 'm':
							std::cout << "You have been hit" << std::endl;
							if(obj)
							obj->hitKart(*Karts[0], 0, tStart);
							break;
						case SDLK_SPACE:
							Players[0]->usePower(Karts,0,tStart,mapObjects);
							
							break;
					}
					break;
				default:
					break;
				
			}
		}
		
		Uint8 *keystate = SDL_GetKeyState(NULL);
	
		if ( keystate[SDLK_UP] ) Karts[0]->move(1);
		if ( keystate[SDLK_DOWN] ) Karts[0]->move(-1);
		if (!keystate[SDLK_UP] && !keystate[SDLK_UP]) Karts[0]->move(0);
		
		if ( keystate[SDLK_LEFT] ){ Karts[0]->rotate(1); anglefile.push(std::pair<float,Uint32>(Karts[0]->getAngle(),tStart));}
		if ( keystate[SDLK_RIGHT] ){ Karts[0]->rotate(-1); anglefile.push(std::pair<float,Uint32>(Karts[0]->getAngle(),tStart));}
		
		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if(d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}
	
	Clean(mapObjects,Players);
	

	return EXIT_SUCCESS;
}

void Game::Clean(std::vector<Object3D*> objs, std::vector<PlayerIA*> players){
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