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

#include "Menu.h"

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

// static const Uint32 WINDOW_WIDTH = 800;
// static const Uint32 WINDOW_HEIGHT = 600;
static const Uint32 WINDOW_BPP = 32;

using namespace glimac;

Game::Game(Race & race, std::vector<PlayerIA*>& players,std::vector<Kart*>& karts):race(race),Players(players), Karts(karts)
{
}


Game::~Game()
{
}

int Game::playChampionShip(){
	std::cout << "Begin Championship" << std::endl;
	int res;
	for(std::vector<Track*>::iterator it = race.getTracks().begin() ; it != race.getTracks().end(); ++it){
			
		res=playTrack(**it);
		if(res!=0)
			break;
		std::cout << "Next Track" << std::endl;
	}
	
	std::cout << "End Championship" << std::endl;
	// CleanAll();
	
}

int Game::playTrack(Track& track){

	//Interface
	Program prog2D;
	prog2D = loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog2D.use();
	
	VBO vbo;
	vbo.bind(GL_ARRAY_BUFFER);
	
	Vertex2DUV vertices[] = {
		Vertex2DUV(-0.5, -0.5, 0.0, 1.0),
		Vertex2DUV(-0.5, 0.5, 0.0, 0.0),
		Vertex2DUV(0.5, 0.5, 1.0, 0.0),
		Vertex2DUV(0.5, -0.5, 1.0, 1.0),
	};
	//on remplit les donnees du bateau
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//on remet le bateau à la mer
	vbo.debind(GL_ARRAY_BUFFER);

	//Création du VAO
	VAO vao;
	//on binde le vao
	vao.bind();
	//on attribue une position 2D qui aura pour id 0
	glEnableVertexAttribArray(0);
	//on attribue une texture qui aura pour id 1
	glEnableVertexAttribArray(1);
	//on remet le bateau au port
	vbo.bind(GL_ARRAY_BUFFER);
	//on définit les paramètres des attributs (position 2D)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, x)));
	//on définit les paramètres des attributs (textures)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, u)));
	//on débinde le VBO
	vbo.debind(GL_ARRAY_BUFFER);
	//on débinde le VAO
	vao.debind();
	
	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog2D.getGLId(), "uTexture");
	
	//On loade l'image
	int img_width=0, img_height=0;
	unsigned char* img;
	img=SOIL_load_image("../textures/menuCircuit800.jpg", &img_width, &img_height, NULL, 0);
	
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	
	Program prog;
	prog = loadProgram("../shaders/3D.vs.glsl","../shaders/tex3D.fs.glsl");
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
	
	// Les power qu'on peut ramasser, a enlever car ils seront inclus dans mapObject
	PowerObject boost(BOOST,1000);
	PowerObject atk_back(ATK_BACK,1000);
	PowerObject trap(TRAP,1000);
	PowerObject shield(SHIELD, 1000);
	PowerObject atk_all(ATK_ALL, 1000);
	PowerObject atk_front(ATK_FRONT, 1000);
	
	//Ciel
	Object3D sky;
	sky.sphere(1,32,16);
	sky.setScale(glm::vec3(70,70,70));
	sky.build();
	sky.LoadTexture("../textures/sky.jpg");
	
	//Lecture du fichier Map
	//Objet temporaire
	Object3D floor;
	floor.LoadObjFromFile("../models/floor.obj");	
	floor.setScale(glm::vec3(16,1,16));
	floor.setHitbox(glm::vec3(0, 0, 0));
	floor.build();
	floor.LoadTexture("../textures/MoonMap.png");
	track.push_back(floor);


	//Element de décor test
	Object3D elementDecor;
	elementDecor.sphere(1,32,16);
	elementDecor.setScale(glm::vec3(1,1,1));
	elementDecor.setPosition(glm::vec3(-30,0,30));
	elementDecor.setHitbox(glm::vec3(0));
	elementDecor.build();
	elementDecor.LoadTexture("../textures/EarthMap.jpg");
	track.push_back(elementDecor);

	Object3D elementDecor2;
	elementDecor2.sphere(1,32,16);
	elementDecor2.setScale(glm::vec3(1,1,1));
	elementDecor2.setPosition(glm::vec3(30,0,30));
	elementDecor2.setHitbox(glm::vec3(0));
	elementDecor2.build();
	elementDecor2.LoadTexture("../textures/EarthMap.jpg");
	track.push_back(elementDecor2);
	
	Object3D elementDecor3;
	elementDecor3.sphere(1,32,16);
	elementDecor3.setScale(glm::vec3(1,1,1));
	elementDecor3.setPosition(glm::vec3(30,0,-30));
	elementDecor3.setHitbox(glm::vec3(0));
	elementDecor3.build();
	elementDecor3.LoadTexture("../textures/EarthMap.jpg");
	track.push_back(elementDecor3);
	
	Object3D elementDecor4;
	elementDecor4.sphere(1,32,16);
	elementDecor4.setScale(glm::vec3(1,1,1));
	elementDecor4.setPosition(glm::vec3(-30,0,-30));
	elementDecor4.setHitbox(glm::vec3(0));
	elementDecor4.build();
	elementDecor4.LoadTexture("../textures/EarthMap.jpg");
	track.push_back(elementDecor4);
	
	//On donne le prochain noeud pour que les IA s'y dirigent
	for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
		(*it)->setNodeTo(track.getNodeStart()->next);
	}
	
	PowerObject *obj=NULL;
	bool done=false;
	int sortie=0;
	
	while(!done) {

		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//A decommenter (Marie)
		// prog2D.use();
		// vao.bind();
		// glUniform1i(locVarTexture,0);
		// glBindTexture(GL_TEXTURE_2D,idMenu);
		// glUniform1i(locVarTexture,0);
		// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		// glBindTexture(GL_TEXTURE_2D,0);
		
		// prog.use();
		
		//Decalage camera
		if(tStart-anglefile.front().second>330){
			if(anglefile.size()>1)
				anglefile.pop();
		}
		//Camera
		ViewMatrix=camera.getViewMatrix(Karts[0]->getPosition(), anglefile.front().first,Karts[0]->back);
	

		//Kart (boucle sur tous les karts)
		for (int id=0;id<8;++id){
			if(id!=0){
				//Deplacement IA
				Karts[id]->moveIA();
			}
				
			Karts[id]->getVAO().bind();		//Bind du VAO
			Karts[id]->TransfoMatrix(ViewMatrix,Karts[id]->getPosition()); //Transformations (View, Translation, anglerotation,scale)
			Karts[id]->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
			Karts[id]->Draw(uTex);	//Draw de l'objet
		}
		
		//Pouvoirs  (boucle sur tous les persos)
		for (std::vector<PlayerIA*>::iterator it = Players.begin() ; it != Players.end(); ++it){
			//Pouvoirs Classiques
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
			//Pouvoirs Speciaux
			if((*it)->getCharacter().isLaunched()){
				if((*it)->getCharacter().isPerimed(tStart)){
					(*it)->getCharacter().useSuperPowerBack((*it)->getKart());
					(*it)->getCharacter().hitSuperPowerBack(Karts);
				}
				else {
					(*it)->getCharacter().continuousHitSuperPower(Karts,(*it)->getKart());
				}
			}
		}
		
		//Dessin des objets de la map
		for (std::vector<Object3D*>::iterator it = track.getMapObjects().begin() ; it != track.getMapObjects().end(); ++it){
			if((*it)->isVisible()){
				(*it)->getVAO().bind();		
				(*it)->movePower();
				(*it)->TransfoMatrix(ViewMatrix, (*it)->getPosition());
				(*it)->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
				(*it)->Draw(uTex);
				
				if((*it)->tooFar()){
					track.getMapObjects().erase(it);	
					delete(*it);
					it--;
				}
			}
			if((*it)->isPerimed(tStart)){
					(*it)->hitKartBack(Karts);
					track.getMapObjects().erase(it);	
					delete(*it);
					it--;
			}
		}


		//Gestion des collisions
		for (int idkart=0;idkart<8;++idkart){	// Boucle sur tous les karts
			//Collision avec les autres objets de la map
			for (std::vector<Object3D*>::iterator it_mapObjects = track.getMapObjects().begin() ; it_mapObjects != track.getMapObjects().end(); ++it_mapObjects){	// Boucle sur tous les objets de la map
				if(Karts[idkart]->isInCollision( **it_mapObjects )){
					Karts[idkart]->avoidCollision( **it_mapObjects );
					Karts[idkart]->move(-1); // Fais ralentir le kart quand il est en collision
					(*it_mapObjects)->hitKart(*Karts[idkart],idkart, tStart);
				}
			}
		
			//Collision avec les autres Karts
			//>>>>>>>>>>>>>>>>>>>>> Boucle sur le reste des karts, permet d'éviter de tester 2 fois la même collision (kart1/kart2 et kart2/kart1 par exemple)
			for (int idotherkart = idkart+1 ; idotherkart <8; ++idotherkart){  // Boucle sur le reste des karts
		        if(Karts[idkart]->isInCollision( *Karts[idotherkart] )){
		        	Karts[idkart]->avoidCollision( *Karts[idotherkart] );
					Karts[idkart]->move(-1); // Fais ralentir le kart quand il est en collision
					
					Players[idkart]->getCharacter().hitSuperPower(tStart, Karts, idotherkart, *Karts[idkart]); //Pouvoir spécial kart1 sur kart2
					Players[idotherkart]->getCharacter().hitSuperPower(tStart, Karts, idkart, *Karts[idotherkart]); //Pouvoir spécial kart2 sur kart1
				}
			}

			/*for (int idotherkart=0;idotherkart<8;++idotherkart){	// Boucle sur le reste des karts
				if(idotherkart==idkart){
					
				}
				else if(Karts[idkart]->isInCollision( *Karts[idotherkart] )){
					Karts[idkart]->avoidCollision( *Karts[idotherkart] );
					Karts[idkart]->move(-1); // Fais ralentir le kart quand il est en collision
					Players[idkart]->getCharacter().hitSuperPower(tStart, Karts, idotherkart, *Karts[idkart]);
					// //Players[idotherkart]->getCharacter().hitSuperPower(tStart, Karts, idkart, *Karts[idotherkart]);
				}
			}*/
		
		}
	
		
		//Sky
		sky.getVAO().bind();		
		sky.setAngle(tStart*0.001f);
		sky.TransfoMatrix(ViewMatrix,sky.getPosition());
		sky.MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
		sky.Draw(uTex);
		
		VAO::debind();
		
		
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {			
				case SDL_QUIT:
					done = true;
					sortie=-1;
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
							Players[0]->usePower(Karts,0,tStart,track.getMapObjects());
							break;
						case SDLK_ESCAPE:
							std::cout << "Pause" << std::endl;
							sortie=0;
							done=true;
							break;
						case 'q':
							Players[0]->getCharacter().useSuperPower(tStart,*Karts[0],track.getMapObjects());
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
	
	CleanObjects(track);
	return sortie;
}

void Game::CleanObjects(Track& track){
	
	// Object3D *obj;
	// while(!track.getMapObjects().empty()){
		// obj=track.getMapObjects().back();
		// track.getMapObjects().pop_back();
		// delete(obj);
	// }
}

void Game::CleanAll(){

	PlayerIA *play;
	while(!Players.empty()){
		play=Players.back();
		Players.pop_back();
		delete(play);
	}
	
	Kart *kart;
	while(!Karts.empty()){
		kart=Karts.back();
		Karts.pop_back();
		delete(kart);
	}
}