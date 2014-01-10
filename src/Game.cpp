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
#include "Rank.h"


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
	track.setNbLaps(1);
	bool raceFinished = false;
	unsigned int timeElapsed = 0;

	//TTF
	TTF_Font *font = NULL;
    font = TTF_OpenFont("../font/CALIBRI.TTF", 12);
    if(font==NULL) std::cout << "ERREUR d'importation de la police" << std::endl;
    
    SDL_Surface* screen = SDL_GetVideoSurface();
    std::vector<SDL_Surface*> rankSurfaces = createRankSurfaces(font);
    std::vector<SDL_Rect> positionSurfaces = createPositions();
	GLuint* idTexture=showRankSurfaces(rankSurfaces, screen, positionSurfaces);
	
	//Interface
	Program prog2D;
	prog2D = loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog2D.use();
	
	
	//Carre qui affiche les pouvoirs
	Object2D powerquad;
	powerquad.setVertices(0.7, 0.9, -0.9, -0.64);
	powerquad.build();
	
	//Carre qui affiche la position
	Object2D positionQuad;
	positionQuad.setVertices(-0.9,-0.7,0.64,0.9);
	positionQuad.build();

	//Surfaces pour afficher le classement final (à optimiser)
	Object2D positionQuad1;
	positionQuad1.setVertices(-0.2,-0.1,0.8,0.9);
	positionQuad1.build();
	Object2D positionQuad2;
	positionQuad2.setVertices(-0.2,-0.1,0.6,0.7);
	positionQuad2.build();
	Object2D positionQuad3;
	positionQuad3.setVertices(-0.2,-0.1,0.4,0.5);
	positionQuad3.build();
	Object2D positionQuad4;
	positionQuad4.setVertices(-0.2,-0.1,0.2,0.3);
	positionQuad4.build();
	Object2D positionQuad5;
	positionQuad5.setVertices(-0.2,-0.1,0.0,0.1);
	positionQuad5.build();
	Object2D positionQuad6;
	positionQuad6.setVertices(-0.2,-0.1,-0.2,-0.1);
	positionQuad6.build();
	Object2D positionQuad7;
	positionQuad7.setVertices(-0.2,-0.1,-0.4,-0.3);
	positionQuad7.build();
	Object2D positionQuad8;
	positionQuad8.setVertices(-0.2,-0.1,-0.6,-0.5);
	positionQuad8.build();

	Object2D persoQuad1;
	persoQuad1.setVertices(0.1,0.2,0.8,0.9);
	persoQuad1.build();
	Object2D persoQuad2;
	persoQuad2.setVertices(0.1,0.2,0.6,0.7);
	persoQuad2.build();
	Object2D persoQuad3;
	persoQuad3.setVertices(0.1,0.2,0.4,0.5);
	persoQuad3.build();
	Object2D persoQuad4;
	persoQuad4.setVertices(0.1,0.2,0.2,0.3);
	persoQuad4.build();
	Object2D persoQuad5;
	persoQuad5.setVertices(0.1,0.2,0.0,0.1);
	persoQuad5.build();
	Object2D persoQuad6;
	persoQuad6.setVertices(0.1,0.2,-0.2,-0.1);
	persoQuad6.build();
	Object2D persoQuad7;
	persoQuad7.setVertices(0.1,0.2,-0.4,-0.3);
	persoQuad7.build();
	Object2D persoQuad8;
	persoQuad8.setVertices(0.1,0.2,-0.6,-0.5);
	persoQuad8.build();
	/*/
	std::vector<Object2D> finalRankQuad;
	for(int i=0; i<8; ++i){
		Object2D quad;
		quad.setVertices(-0.2,-0.1,-0.8+i*0.1,-0.7+i*0.1);
		quad.build();
		finalRankQuad.push_back(quad);
	}
	std::vector<Object2D> finalPersoQuad;
	for(int i=0; i<8; ++i){
		Object2D quad;
		quad.setVertices(0.1,0.2,-0.8+i*0.1,-0.7+i*0.1);
		quad.build();
		finalPersoQuad.push_back(quad);
	}*/
	
	
	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog2D.getGLId(), "uTexture");
	GLuint* texturepower=PowerTexture();
	GLuint* textureRank=RankTexture();
	GLuint* texturePerso=PersoTexture();
	
	Program prog;
	prog = loadProgram("../shaders/3D.vs.glsl","../shaders/tex3D.fs.glsl");
	prog.use();
	
	GLuint uMVPMatrix=glGetUniformLocation(prog.getGLId(),"uMVPMatrix");
	GLuint uMVMatrix=glGetUniformLocation(prog.getGLId(),"uMVMatrix");
	GLuint uNormalMatrix=glGetUniformLocation(prog.getGLId(),"uNormalMatrix");
	GLuint uTex=glGetUniformLocation(prog.getGLId(),"uTexture");
	
	GLuint uKd=glGetUniformLocation(prog.getGLId(),"uKd");
	GLuint uKs=glGetUniformLocation(prog.getGLId(),"uKs");
	GLuint uShininess=glGetUniformLocation(prog.getGLId(),"uShininess");
	GLuint uLightDir_vs=glGetUniformLocation(prog.getGLId(),"uLightDir_vs");
	GLuint uLightIntensity=glGetUniformLocation(prog.getGLId(),"uLightIntensity");
	
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
	sky.setHitbox(glm::vec3(0));
	sky.build();
	sky.LoadTexture("../textures/sky.jpg");
	
	PowerObject *obj=NULL;
	bool done=false;
	bool ready=false;
	int sortie=0;
	
	std::cout << "Lecture du fichier Map" << std::endl;
	//Lecture du fichier Map
	track.insertElt();

	//Nodes

	unsigned int rank = 8;
	//On donne le prochain noeud pour que les IA s'y dirigent
	for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
		(*it)->setNodeTo(track.getNodeStart()->next);
		(*it)->setRank(rank);
		rank--;
	}
	
		 Node* currentNode = track.getNodeStart();
		 Object3D* node;
	 // while(1){
		 // node = new Object3D();
		 // node->sphere(1,32,16);
		 // node->setScale(glm::vec3(1,1,1));
		 // node->setPosition(currentNode->getPosition());
		 // node->build();
		 // if(currentNode==track.getNodeStart())
		 // node->LoadTexture("../textures/Maison.jpg");
		 // else
		 // node->LoadTexture("../textures/CCTex.jpg");
		 // track.push_back(*node);
		 // currentNode = currentNode->next;
		 // if(currentNode==track.getNodeStart())
			 // break;
	 // }

	placementKart(track.getNodeStart());

	glUniform3f(uKd, 1, 1, 1);
	glUniform3f(uKs, 1, 1, 1);
	glUniform1f(uShininess, 32);

	glUniform3f(uLightIntensity, 3, 3, 3);
		
	timeElapsed=0;
	while(!done) {

		Uint32 tStart = SDL_GetTicks();
		
		if(timeElapsed >= FPS && timeElapsed < 2*FPS)
			std::cout << "3 ..." << std::endl;
		if(timeElapsed >= 2*FPS && timeElapsed < 3*FPS)
			std::cout << "2 ..." << std::endl;
		if(timeElapsed >= 3*FPS && timeElapsed < 4*FPS)
			std::cout << "1 ..." << std::endl;

		if(timeElapsed >= 4*FPS && timeElapsed < 5*FPS){
			std::cout << "PINAAAAAAAAAAGE !!!" << std::endl;
			ready=true;
		}
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		prog2D.use();
		//affichage des pouvoirs
		 powerquad.initDraw();
		if (Players[0]->getPower()==NULL){
			powerquad.bindTex(texturepower[0]);
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

		//Gestion du classement
		if( !raceFinished ){
			ranking(Karts);
			positionQuad.initDraw();		
			positionQuad.bindTex(textureRank[Karts[0]->getRank()-1]);
			positionQuad.Draw(locVarTexture);
		}
		else{
			getFinalRanking(Players);

			positionQuad1.initDraw();		
			positionQuad1.bindTex(textureRank[0]);
			positionQuad1.Draw(locVarTexture);
			positionQuad2.initDraw();		
			positionQuad2.bindTex(textureRank[1]);
			positionQuad2.Draw(locVarTexture);
			positionQuad3.initDraw();		
			positionQuad3.bindTex(textureRank[2]);
			positionQuad3.Draw(locVarTexture);
			positionQuad4.initDraw();		
			positionQuad4.bindTex(textureRank[3]);
			positionQuad4.Draw(locVarTexture);
			positionQuad5.initDraw();		
			positionQuad5.bindTex(textureRank[4]);
			positionQuad5.Draw(locVarTexture);
			positionQuad6.initDraw();		
			positionQuad6.bindTex(textureRank[5]);
			positionQuad6.Draw(locVarTexture);
			positionQuad7.initDraw();		
			positionQuad7.bindTex(textureRank[6]);
			positionQuad7.Draw(locVarTexture);
			positionQuad8.initDraw();		
			positionQuad8.bindTex(textureRank[7]);
			positionQuad8.Draw(locVarTexture);

			persoQuad1.initDraw();		
			persoQuad1.bindTex(texturePerso[getIdPersoByRank(Players, 1)]);
			persoQuad1.Draw(locVarTexture);
			persoQuad2.initDraw();		
			persoQuad2.bindTex(texturePerso[getIdPersoByRank(Players, 2)]);
			persoQuad2.Draw(locVarTexture);
			persoQuad3.initDraw();		
			persoQuad3.bindTex(texturePerso[getIdPersoByRank(Players, 3)]);
			persoQuad3.Draw(locVarTexture);
			persoQuad4.initDraw();		
			persoQuad4.bindTex(texturePerso[getIdPersoByRank(Players, 4)]);
			persoQuad4.Draw(locVarTexture);
			persoQuad5.initDraw();		
			persoQuad5.bindTex(texturePerso[getIdPersoByRank(Players, 5)]);
			persoQuad5.Draw(locVarTexture);
			persoQuad6.initDraw();		
			persoQuad6.bindTex(texturePerso[getIdPersoByRank(Players, 6)]);
			persoQuad6.Draw(locVarTexture);
			persoQuad7.initDraw();		
			persoQuad7.bindTex(texturePerso[getIdPersoByRank(Players, 7)]);
			persoQuad7.Draw(locVarTexture);
			persoQuad8.initDraw();		
			persoQuad8.bindTex(texturePerso[getIdPersoByRank(Players, 8)]);
			persoQuad8.Draw(locVarTexture);
		} 

		prog.use();
			
		//Decalage camera
		if(tStart-anglefile.front().second>330){
			if(anglefile.size()>1)
				anglefile.pop();
		}
		//Camera
		ViewMatrix=camera.getViewMatrix(Karts[0]->getPosition(), anglefile.front().first,Karts[0]->back);
		
		glUniform3fv(uLightDir_vs, 1, glm::value_ptr(glm::vec3(ViewMatrix * glm::vec4(3, 3, 3, 1))));
		
		//Kart (boucle sur tous les karts)
		for (int id=0;id<8;++id){
			 if((id!=0 && ready) || raceFinished ){

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
				(*it)->rotateObj();
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

			//Collision avec les bords de la map
			Karts[idkart]->collisionWithMap(track.getLargeur(), track.getLongueur());
		
		}


/*		for(int i=0; i<8; ++i){
			finalRankQuad[i].initDraw();		
			finalRankQuad[i].bindTex(textureRank[i]);
			finalRankQuad[i].Draw(locVarTexture);
		}
*/		
		//getFinalRanking(Players, textSurfaces, font);

		// showRankSurfaces(rankSurfaces, screen, positionSurfaces);

		// std::cout << "Votre classement : " << Karts[0]->getRank() << std::endl;
		//std::cout << "NbNodesPassed : " << Karts[0]->getNbNodesPassed() << std::endl;

		if((float)Karts[0]->getNbNodesPassed()/track.getNbNodes() == track.getNbLaps() ){
			raceFinished = true;
		}
		
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
							if(ready)
								Players[0]->usePower(Karts,0,tStart,track.getMapObjects());
							break;
						case SDLK_ESCAPE:
							std::cout << "Pause" << std::endl;
							sortie=0;
							done=true;
							break;
						case 'q':
							if(ready)
								Players[0]->getCharacter().useSuperPower(tStart,*Karts[0],track.getMapObjects());
							break;
					}
					break;
				default:
					break;
				
			}
		}
		
		Uint8 *keystate = SDL_GetKeyState(NULL);
		if(ready && !raceFinished){
			if ( keystate[SDLK_UP] ) Karts[0]->move(1);
			if ( keystate[SDLK_DOWN] ) Karts[0]->move(-1);
			if (!keystate[SDLK_UP] && !keystate[SDLK_UP]) Karts[0]->move(0);
			
			if ( keystate[SDLK_LEFT] ){ Karts[0]->rotate(1); anglefile.push(std::pair<float,Uint32>(Karts[0]->getAngle(),tStart));}
			if ( keystate[SDLK_RIGHT] ){ Karts[0]->rotate(-1); anglefile.push(std::pair<float,Uint32>(Karts[0]->getAngle(),tStart));}
		}
		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if(d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
		
		timeElapsed++;
	}
	std::cout << "Nettoyage" << std::endl;
	CleanObjects(track);
	TTF_CloseFont(font);

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



void Game::placementKart(Node *nodeStart){
	Node* nodeTo=nodeStart->next;
	float angleNode,x,z;
	// glm::vec3 direction=glm::vec3(nodeStart->getPosition().x-nodeTo->getPosition().x,0,nodeStart->getPosition().z-nodeTo->getPosition().z);
	// direction=glm::normalize(direction);
	glm::vec3 dirVect = glm::vec3(nodeTo->getPosition().x-nodeStart->getPosition().x, 0, nodeTo->getPosition().z-nodeStart->getPosition().z);
	dirVect=glm::normalize(dirVect);
	glm::vec3 defautVect = glm::vec3(0,0,1);
	float angleToNode = acos(glm::dot(defautVect, dirVect))*180/M_PI;	
				
	for (std::vector<Kart*>::iterator it = Karts.begin() ; it != Karts.end(); ++it){
		(*it)->setDirection(dirVect);
		(*it)->setAngle(angleToNode);

		float x_first=nodeStart->getLeftPos().x-nodeStart->getPosition().x;
		float z_first=nodeStart->getLeftPos().z-nodeStart->getPosition().z;
		glm::vec3 positionFirst = glm::vec3(nodeStart->getPosition().x+x_first/4.0,0,nodeStart->getPosition().z+z_first/4.0); 

		float x_second=nodeStart->getRightPos().x-nodeStart->getPosition().x;
		float z_second=nodeStart->getRightPos().z-nodeStart->getPosition().z;
		glm::vec3 positionSecond = glm::vec3(nodeStart->getPosition().x+x_second/4.0,0,nodeStart->getPosition().z+z_second/4.0);
			

		switch((*it)->getRank()){
			case 1 :
				(*it)->setPosition(positionFirst);
			break;
			case 2 :
				(*it)->setPosition(positionSecond);
			break;
			case 3 :
				(*it)->setPosition(glm::vec3(positionFirst.x-5*dirVect.x,0,positionFirst.z-5*dirVect.z));
			break;
			case 4 :
				(*it)->setPosition(glm::vec3(positionSecond.x-7*dirVect.x,0,positionSecond.z-7*dirVect.z));
			break;
			case 5 : 
				(*it)->setPosition(glm::vec3(positionFirst.x-10*dirVect.x,0,positionFirst.z-10*dirVect.z));
			break;
			case 6 : 
				(*it)->setPosition(glm::vec3(positionSecond.x-12*dirVect.x,0,positionSecond.z-12*dirVect.z));
			break;
			case 7 :
				(*it)->setPosition(glm::vec3(positionFirst.x-15*dirVect.x,0,positionFirst.z-15*dirVect.z));
			break;
			case 8 :
				(*it)->setPosition(glm::vec3(positionSecond.x-17*dirVect.x,0,positionSecond.z-17*dirVect.z));
			break;
		}
	}
}

