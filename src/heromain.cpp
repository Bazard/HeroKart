#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "VBO.hpp"
#include "VAO.hpp"
#include "Vertex3DRGB.hpp"
#include "Program.hpp"
#include "Shader.hpp"
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
#include "Object3D.h"
#include "Kart.h"
// #include "Menu.h"

#undef main

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_WIDTH = 800;
static const Uint32 WINDOW_HEIGHT = 600;
static const Uint32 WINDOW_BPP = 32;

using namespace glimac;

//void setupColor2DShaders();
std::vector<Vertex3DRGB> computeDiscVertices(unsigned int discretization);

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

	//Objet1
	Kart kart(2,0.01,0.75);
	kart.setPosition(glm::vec3(0,0.5,0));
	kart.LoadObjFromFile("../models/ACC/ACC2.obj");	
	kart.buildVBO();
	kart.buildVAO();
	kart.LoadTexture("../textures/CCTex.jpg");
	
	//Objet2
	Object3D floor;
	floor.LoadObjFromFile("../models/floor.obj");	
	floor.build();
	floor.LoadTexture("../textures/MoonMap.png");

	//Ciel
	Object3D sky;
	sky.sphere(1,32,16);
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
	glm::mat4 ProjMatrix; 
	glm::mat4 NormalMatrix;
		
	TrackballCamera camera;
	 //FreeflyCamera camera;
	
	// Menu interface;
	// interface.RocketInitialisation();
	
	int xinit=WINDOW_WIDTH/2;
	int yinit=WINDOW_HEIGHT/2;
	
	int xcurr,ycurr;
	bool sens=true;
	bool done=false;
	while(!done) {
		
		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// SDL_GetMouseState(&xcurr, &ycurr);
		// camera.rotateLeft((float)(xinit-xcurr));
		// camera.rotateUp((float)(yinit-ycurr));
		yinit=ycurr;
		xinit=xcurr;
		
		
		//Camera
		ViewMatrix=camera.getViewMatrix(kart.getPosition(), kart.getAngle(),kart.back);
		ProjMatrix=glm::perspective(70.f,(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1f,100.f); //angle vertical, ratio largeur/hauteur, near, far
		
		//Objet1
		kart.getVAO().bind();		
		
		kart.TransfoMatrix(ViewMatrix, kart.getPosition(), kart.getAngle(), glm::vec3(1,1,1));
		
		NormalMatrix=glm::transpose(glm::inverse(kart.MVMatrix));
	
		glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(kart.MVMatrix));
		glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix*kart.MVMatrix));			
		glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
		kart.Draw(uTex);

		
		
		//Floor
		floor.getVAO().bind();		
		floor.MVMatrix = glm::translate(ViewMatrix, glm::vec3(0,0,-1));
		floor.MVMatrix = glm::scale(floor.MVMatrix, glm::vec3(4, 1, 4));
		NormalMatrix=glm::transpose(glm::inverse(floor.MVMatrix));
	
		glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(floor.MVMatrix));
		glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix*floor.MVMatrix));			
		glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
		floor.Draw(uTex);
		

		//Sky
		sky.getVAO().bind();		
		sky.MVMatrix = glm::rotate(ViewMatrix, tStart* 0.001f, glm::vec3(0,1,0));
		sky.MVMatrix = glm::scale(sky.MVMatrix, glm::vec3(20, 20, 20));
		NormalMatrix=glm::transpose(glm::inverse(sky.MVMatrix));
	
		glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(floor.MVMatrix));
		glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix*sky.MVMatrix));			
		glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
		sky.Draw(uTex);
		
		VAO::debind();
	
		// interface.RocketShow();
		
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_MOUSEBUTTONDOWN:
					// if (e.button.button==SDL_BUTTON_LEFT){
						
					// }
					// else if(e.button.button==SDL_BUTTON_RIGHT){
						// xinit=(float)(e.button.x);
						// yinit=(float)(e.button.y);		
					// }
			
					break;
				
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
				
			}
		}
		
		Uint8 *keystate = SDL_GetKeyState(NULL);
		// if ( keystate[SDLK_w] ) camera.moveFront(0.1);
		// if ( keystate[SDLK_a] ) camera.moveLeft(0.1);
		// if ( keystate[SDLK_s] ) camera.moveFront(-0.1);
		// if ( keystate[SDLK_d] ) camera.moveLeft(-0.1);
	
		if ( keystate[SDLK_UP] ) kart.move(1);
		if ( keystate[SDLK_DOWN] ) kart.move(-1);
		if (!keystate[SDLK_UP] && !keystate[SDLK_UP]) kart.move(0);
		
		if ( keystate[SDLK_LEFT] ) kart.rotate(1);
		if ( keystate[SDLK_RIGHT] ) kart.rotate(-1);
		
		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if(d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}
	
	// interface.RocketShutDown();
	
	SDL_Quit();

	return EXIT_SUCCESS;
}

std::vector<Vertex3DRGB> computeDiscVertices(unsigned int discretization){
	std::vector<Vertex3DRGB> vertices;
	srand(time(0));
	
	//Constructeur à appeler
	for(float i=0.0;i<2*M_PI;i+=(2*M_PI/discretization)){
			//Point 1
			vertices.push_back(Vertex3DRGB(cos(i)*0.5,sin(i)*0.5,(float)(rand()%255)/255,0,(float)(rand()%255)/255,(float)(rand()%255)/255));

			//Point 2
			vertices.push_back(Vertex3DRGB(cos(i+2*M_PI/discretization)*0.5,sin(i+2*M_PI/discretization)*0.5,0,(float)(rand()%255)/255,(float)(rand()%255)/255,(float)(rand()%255)/255));

			//Point 3 (centre)
			vertices.push_back(Vertex3DRGB((0.0),(0.0),0,(float)(rand()%255)/255,(float)(rand()%255)/255,(float)(rand()%255)/255));

	}
	return vertices;
}