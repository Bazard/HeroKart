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
#include "Object2D.h"
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
	CleanAll();
	
}

int Game::playTrack(Track& track){

	//Interface
	Program prog2D;
	prog2D = loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog2D.use();
	
	
	//Carre qui affiche les pouvoirs
	Object2D powerquad;
	powerquad.setVertices(0.7, 0.9, -0.9, -0.64);
	powerquad.build();
	
	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog2D.getGLId(), "uTexture");
	GLuint* texturepower=PowerTexture();
	
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
	
	// Les power qu'on peut ramasser, a enlever car ils seront inclus dans powObject
	PowerObject boost(BOOST,10000);
	PowerObject atk_back(ATK_BACK,10000);
	PowerObject trap(TRAP,10000);
	PowerObject shield(SHIELD, 10000);
	PowerObject atk_all(ATK_ALL, 10000);
	PowerObject atk_front(ATK_FRONT, 10000);
	
	//Ciel
	Object3D sky;
	sky.sphere(1,32,16);
	sky.setScale(glm::vec3(80,80,80));
	sky.build();
	sky.LoadTexture("../textures/sky.jpg");

	unsigned int rank = 1;
	
	PowerObject *obj=NULL;
	bool done=false;
	int sortie=0;
	
	std::cout << "Lecture du fichier Map" << std::endl;
	//Lecture du fichier Map
	track.insertElt();
	
	//Nodes
	Node* currentNode = track.getNodeStart();
	Object3D* node;
	
	//On donne le prochain noeud pour que les IA s'y dirigent
	for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
		(*it)->setNodeTo(track.getNodeStart()->next);
		(*it)->setRank(rank);
		rank++;
	}
	
	
	for(int i=0; i<4; ++i){
		node = new Object3D();
		node->sphere(1,32,16);
		node->setScale(glm::vec3(1,1,1));
		node->setPosition(currentNode->getPosition());
		node->build();
		node->LoadTexture("../textures/CCTex.jpg");
		track.push_back(*node);
		currentNode = currentNode->next;
	}
	


	while(!done) {

		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		prog2D.use();
		//affichage des pouvoirs
		 powerquad.initDraw(locVarTexture);
		if (Players[0]->getPower()==NULL){
			powerquad.bindTex(texturepower[6]);
		}
		else{
			switch(Players[0]->getPower()->getType()){
				case BOOST:
					powerquad.bindTex(texturepower[0]);
					break;
				case ATK_FRONT:
					powerquad.bindTex(texturepower[1]);
					break;
				case ATK_BACK: 
					powerquad.bindTex(texturepower[2]);
					break;
				case ATK_ALL:
					powerquad.bindTex(texturepower[3]);
					break;
				case SHIELD:
					powerquad.bindTex(texturepower[4]);
					break;
				case TRAP:
					powerquad.bindTex(texturepower[5]);
					break;
			}
		}
		powerquad.Draw(locVarTexture);

		prog.use();
			
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
				int sortie=Karts[id]->moveIA(track.getMapObjects(),track.getPowObjects(), Karts,Players[id]->getPower(),
				Players[id]->getCharacter().getHero(), Players[id]->getCharacter().isPowerReady(tStart));
					if(sortie==1)
						Players[id]->usePower(Karts,id,tStart,track.getMapObjects());
					else if(sortie==2)
						Players[id]->getCharacter().useSuperPower(tStart,*Karts[id],track.getMapObjects());
			 }else{
			 	Karts[id]->crossANode();
			 }
				
			Karts[id]->getVAO().bind();		//Bind du VAO
			Karts[id]->TransfoMatrix(ViewMatrix,Karts[id]->getPosition()); //Transformations (View, Translation, anglerotation,scale)
			Karts[id]->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
			Karts[id]->Draw(uTex);	//Draw de l'objet
			
			//Pouvoirs Classiques
			if(Players[id]->getPower()){
				if(Players[id]->getPower()->isLaunched() && Players[id]->getPower()->withKart()){
					Players[id]->getPower()->getVAO().bind();		//Bind du VAO
					Players[id]->getPower()->TransfoMatrix(ViewMatrix, Players[id]->getKart().getPosition()); //Transformations (View, Translation, anglerotation,scale)
					Players[id]->getPower()->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT); //Envoi des matrices au shader
					Players[id]->getPower()->Draw(uTex);	//Draw de l'objet
				}
				
				if(Players[id]->getPower()->isLaunched() && Players[id]->getPower()->isPerimed(tStart)){
					Players[id]->stopPower(Karts,id);
				}			
			}
			//Pouvoirs Speciaux
			if(Players[id]->getCharacter().isLaunched()){
				if(Players[id]->getCharacter().isPerimed(tStart)){
					Players[id]->getCharacter().useSuperPowerBack(Players[id]->getKart());
					Players[id]->getCharacter().hitSuperPowerBack(Karts);
				}
				else {
					Players[id]->getCharacter().continuousHitSuperPower(Karts,Players[id]->getKart());
				}
			}
		}
		
		//Dessin des objets de la map
		for(int id=0;id<track.getMapObjects().size();++id){
			if(track.getMapObjects()[id]->isVisible()){
				track.getMapObjects()[id]->getVAO().bind();	
				track.getMapObjects()[id]->movePower();
				track.getMapObjects()[id]->TransfoMatrix(ViewMatrix, track.getMapObjects()[id]->getPosition());
				track.getMapObjects()[id]->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
				track.getMapObjects()[id]->Draw(uTex);
				
			}
			if(track.getMapObjects()[id]->tooFar()){
					delete(track.getMapObjects()[id]);
					track.getMapObjects().erase(track.getMapObjects().begin()+id);	
					id--;
			}
			else if(track.getMapObjects()[id]->isPerimed(tStart)){
					track.getMapObjects()[id]->hitKartBack(Karts);
					delete(track.getMapObjects()[id]);
					track.getMapObjects().erase(track.getMapObjects().begin()+id);	
					id--;
			}
		}
		
		//Dessin des pouvoirs de la map
		for (std::vector<PowerObject*>::iterator it = track.getPowObjects().begin() ; it != track.getPowObjects().end(); ++it){
			if((*it)->isVisible()){
				(*it)->getVAO().bind();		
				(*it)->TransfoMatrix(ViewMatrix, (*it)->getPosition());
				(*it)->MatrixToShader(uMVMatrix, uMVPMatrix, uNormalMatrix, WINDOW_WIDTH, WINDOW_HEIGHT);
				(*it)->Draw(uTex);
			}
			if((*it)->isPerimed(tStart)){
					(*it)->visible=true;
			}
		}

		//Gestion des collisions
		for (int idkart=0;idkart<8;++idkart){	// Boucle sur tous les karts
			//Collision avec les autres objets de la map
			for (std::vector<Object3D*>::iterator it_mapObjects = track.getMapObjects().begin() ; it_mapObjects != track.getMapObjects().end(); ++it_mapObjects){	// Boucle sur tous les objets de la map
				if(Karts[idkart]->isInCollision( **it_mapObjects )){
					
					if(((*it_mapObjects)->hitKart(*Karts[idkart],idkart, tStart))==0){
						Karts[idkart]->avoidCollision( **it_mapObjects );
						Karts[idkart]->move(-1); // Fais ralentir le kart quand il est en collision
					}
				}
			}
		
			//Collision avec les pouvoirs ramassables de la map
			for (std::vector<PowerObject*>::iterator it_powObjects = track.getPowObjects().begin() ; it_powObjects != track.getPowObjects().end(); ++it_powObjects){	// Boucle sur tous les objets de la map
				if(Karts[idkart]->isInCollision( **it_powObjects )){
					Players[idkart]->pickPower(**it_powObjects,tStart);
					(*it_powObjects)->visible=false;
				}
			}
			
			//Collision avec les autres Karts
			//>>>>>>>>>>>>>>>>>>>>> Boucle sur le reste des karts, permet d'éviter de tester 2 fois la même collision (kart1/kart2 et kart2/kart1 par exemple)
			for (int idotherkart = idkart+1 ; idotherkart <8; ++idotherkart){  // Boucle sur le reste des karts
				if(Karts[idkart]->intouchable || Karts[idotherkart]->intouchable)
					continue;
					
		        if(Karts[idkart]->isInCollision( *Karts[idotherkart] )){
		        	Karts[idkart]->avoidCollision( *Karts[idotherkart] );
					Karts[idkart]->move(-1); // Fais ralentir le kart quand il est en collision
					
					Players[idkart]->getCharacter().hitSuperPower(tStart, Karts, idotherkart, *Karts[idkart]); //Pouvoir spécial kart1 sur kart2
					Players[idotherkart]->getCharacter().hitSuperPower(tStart, Karts, idkart, *Karts[idotherkart]); //Pouvoir spécial kart2 sur kart1
				}
			}
		
		}


		//Gestion du classement
		ranking(Karts);
		// std::cout << "Votre classement : " << Karts[0]->getRank() << std::endl;
		//std::cout << "NbNodesPassed : " << Karts[0]->getNbNodesPassed() << std::endl;

	
		
		//Sky
		sky.getVAO().bind();		
		sky.setAngle(tStart*0.001f);
		sky.TransfoMatrix(ViewMatrix,Karts[0]->getPosition());
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
							Players[0]->pickPower(atk_front,tStart);
							obj=Players[0]->getPower();
							break;
						case 'b':
							std::cout << "You pick a Boost" << std::endl;
							Players[0]->pickPower(boost,tStart);
							break;
						case 'v':
							std::cout << "You pick an Attack_back" << std::endl;
							Players[0]->pickPower(atk_back,tStart);
							obj=Players[0]->getPower();
							break;
						case 'c':
							std::cout << "You pick a Trap" << std::endl;
							Players[0]->pickPower(trap,tStart);
							obj=Players[0]->getPower();
							break;
						case 'x':
							std::cout << "You pick a Shield" << std::endl;
							Players[0]->pickPower(shield,tStart);
							break;
						case 'z':
							std::cout << "You pick an Attack_All" << std::endl;
							Players[0]->pickPower(atk_all,tStart);
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
	std::cout << "Nettoyage" << std::endl;
	CleanObjects(track);

	return sortie;
}

void Game::CleanObjects(Track& track){
	
// for (std::vector<Object3D*>::iterator it = track.getMapObjects().begin() ; it != track.getMapObjects().end(); ++it)
	// delete(*it);
// track.getMapObjects().clear();

// for (std::vector<PowerObject*>::iterator it = track.getPowObjects().begin() ; it != track.getPowObjects().end(); ++it)
	// delete(*it);
// track.getPowObjects().clear();

}

void Game::CleanAll(){

for (std::vector<PlayerIA*>::iterator it = Players.begin() ; it != Players.end(); ++it)
	delete(*it);
Players.clear();

for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it)
	delete(*it);
Karts.clear();
}


// Gère le classement de la course
void Game::ranking(std::vector<Kart*>& karts){
	// Boucle sur tous les karts
	for(std::vector<Kart*>::iterator currKart = karts.begin() ; currKart != karts.end(); ++currKart){

		// Boucle sur tous les autres karts
		for(std::vector<Kart*>::iterator otherKart = currKart+1 ; otherKart != karts.end(); ++otherKart){
			// Si currKart a passé moins de noeuds que otherKart
			if( (*currKart)->getNbNodesPassed() < (*otherKart)->getNbNodesPassed() ){
				// Si currKart est devant otherKart
				if( (*currKart)->getRank() < (*otherKart)->getRank() ){
					(*currKart)->incrRank();
					(*otherKart)->decrRank();
				}else{

				}
			}// Si currKart a passé plus de noeuds que otherKart
			else if( (*currKart)->getNbNodesPassed() > (*otherKart)->getNbNodesPassed() ){
				// Si currKart est derriere otherKart
				if( (*currKart)->getRank() > (*otherKart)->getRank() ){
					(*currKart)->decrRank();
					(*otherKart)->incrRank();
				}else{

				}
			}// Si currKart et otherKart ont franchi le même nombre de noeuds
			else if( (*currKart)->getNbNodesPassed() == (*otherKart)->getNbNodesPassed() ){
				// Si currKart est plus proche du noeud que otherKart
				if( (*currKart)->distanceToNextNode() < (*otherKart)->distanceToNextNode() ){
					// Si currKart est derrière otherKart
					if( (*currKart)->getRank() > (*otherKart)->getRank() ){
						(*currKart)->decrRank();
						(*otherKart)->incrRank();
					}else{

					}
				}// Si currKart est plus loin du noeud que otherKart
				else if( (*currKart)->distanceToNextNode() > (*otherKart)->distanceToNextNode() ){
					// Si currKart est devant otherKart
					if( (*currKart)->getRank() < (*otherKart)->getRank() ){
						(*currKart)->incrRank();
						(*otherKart)->decrRank();
					}else{

					}
				}
			}
		}//for tous les autres karts
	}//for tous les karts
}
