#include <iostream>
#include <cstdlib>
#include <cmath>
#include "VBO.hpp"
#include "VAO.hpp"
#include <vector>
#include <ctime>
#include "PlayerIA.h"
#include "Character.h"
#include "Race.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "Program.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Vertex2DUV.hpp"
#include <SOIL/SOIL.h>
#include "Menu.h"

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

using namespace glimac;

int menuCircuit(std::vector<Character*>& character,std::vector<Track*>& track){
	int sortie=-1;
	VBO vbo;
	vbo.bind(GL_ARRAY_BUFFER);
	
	Vertex2DUV vertices[] = {
		Vertex2DUV(-1, -1, 0.0, 1.0),
		Vertex2DUV(-1, 1, 0.0, 0.0),
		Vertex2DUV(1, 1, 1.0, 0.0),
		Vertex2DUV(1, -1, 1.0, 1.0),
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

	Program prog;
	prog= loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog.use();

	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog.getGLId(), "uTexture");

	//On loade l'image
	int img_width=0, img_height=0;
	unsigned char* img = SOIL_load_image("../textures/menuCircuit800.jpg", &img_width, &img_height, NULL, 0);
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);

	Track* circuit1=new Track("village");
	Track* circuit2=new Track("montreal");
	Track* circuit3=new Track("forteresse");
		
	bool done = false;
	while (!done) {
		Uint32 tStart = SDL_GetTicks();
		/* Rendering code goes here */
		//on nettoie la fenêtre
		glClear(GL_COLOR_BUFFER_BIT);
		//on rebinde le vao
		vao.bind();
		glUniform1i(locVarTexture,0);
		//Premier triangle
		glBindTexture(GL_TEXTURE_2D,idMenu);
		glUniform1i(locVarTexture,0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindTexture(GL_TEXTURE_2D,0);

		//on débinde le vao
		vao.debind();

		// Application code goes here

		SDL_Event e;
		int xClicked, yClicked;
		while (SDL_PollEvent(&e)) {

			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button==SDL_BUTTON_LEFT){
						xClicked=(float)(e.button.x);
						yClicked=(float)(e.button.y);
						if (xClicked>=88 && xClicked <=277 && yClicked>=226 && yClicked<=416){
							sortie=0;
							done=true;
						}
						else if (xClicked>=298 && xClicked <=492 && yClicked>=226 && yClicked<=416){
							sortie=1;
							done=true;
						}
						else if (xClicked>=512 && xClicked <=705 && yClicked>=226 && yClicked<=416){
							sortie=2;
							done=true;
						}
						else if (xClicked>=68 && xClicked <=148 && yClicked>=38 && yClicked<=88){
							sortie=3;
							done = true;
						}
					}
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
				}
		}

		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if (d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}

	glDeleteTextures(1,&idMenu);
	
	switch(sortie){
		case -1:
			break;
		case 0:
			std::cout << "bonjour" << std::endl;
			track.push_back(circuit1);
			track.push_back(circuit2);
			track.push_back(circuit3);
			break;
		case 1:
			std::cout << "bonjour2" << std::endl;
			track.push_back(circuit2);
			track.push_back(circuit1);
			track.push_back(circuit3);
			break;
		case 2:
			std::cout << "bonjour3" << std::endl;
			track.push_back(circuit3);
			track.push_back(circuit2);
			track.push_back(circuit1);
			break;
		case 3:
			track.clear();
			character.clear();
			sortie=menuPersonnage(character,track);
			break;
	}
	
	return sortie;
}

int menuPersonnage(std::vector<Character*>& character, std::vector<Track*>& track){
	int sortie=-1;
	VBO vbo;
	vbo.bind(GL_ARRAY_BUFFER);
	
	Vertex2DUV vertices[] = {
		Vertex2DUV(-1, -1, 0.0, 1.0),
		Vertex2DUV(-1, 1, 0.0, 0.0),
		Vertex2DUV(1, 1, 1.0, 0.0),
		Vertex2DUV(1, -1, 1.0, 1.0),
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

	Program prog;
	prog= loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog.use();

	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog.getGLId(), "uTexture");

	//On loade l'image
	int img_width=0, img_height=0;
	unsigned char* img = SOIL_load_image("../textures/menuPersonnage800.jpg", &img_width, &img_height, NULL, 0);
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);

	bool done = false;
	while (!done) {
		Uint32 tStart = SDL_GetTicks();
		/* Rendering code goes here */
		//on nettoie la fenêtre
		glClear(GL_COLOR_BUFFER_BIT);
		//on rebinde le vao
		vao.bind();
		glUniform1i(locVarTexture,0);
		//Premier triangle
		glBindTexture(GL_TEXTURE_2D,idMenu);
		glUniform1i(locVarTexture,0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindTexture(GL_TEXTURE_2D,0);

		//on débinde le vao
		vao.debind();

		// Application code goes here
		Character *john=new Character(JOHN,1000);
		Character *klaus=new Character(KLAUS,1000);
		Character *doug=new Character(DOUG,1000);
		Character *stan=new Character(STAN,1000);
		Character *steve=new Character(STEVE,1000);
		Character *burt=new Character(BURT,1000);
		Character *mckormack=new Character(MCKORMACK,1000);
		Character *jennifer=new Character(JENNIFER,1000);

		SDL_Event e;
		int xClicked, yClicked;
		while (SDL_PollEvent(&e)) {

			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button==SDL_BUTTON_LEFT){
						xClicked=(float)(e.button.x);
						yClicked=(float)(e.button.y);
						if (xClicked>=86 && xClicked <=229 && yClicked>=136 && yClicked<=324){
							sortie=0;
							std::cout << "Vous avez choisi John" << std::endl;
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);							
							done=true;
						}
						else if (xClicked>=248 && xClicked <=389 && yClicked>=136 && yClicked<=324){
							std::cout << "Vous avez choisi Klaus" << std::endl;
							sortie=0;
							character.push_back(klaus);
							character.push_back(john);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=406 && xClicked <=550 && yClicked>=136 && yClicked<=324){
							std::cout << "Vous avez choisi Doug" << std::endl;
							sortie=0;
							character.push_back(doug);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=565 && xClicked <=710 && yClicked>=136 && yClicked<=324){
							std::cout << "Vous avez choisi Stan" << std::endl;
							sortie=0;
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=86 && xClicked <=229 && yClicked>=338 && yClicked<=527){
							std::cout << "Vous avez choisi Steve" << std::endl;
							sortie=0;
							character.push_back(steve);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=248 && xClicked <=389 && yClicked>=338 && yClicked<=527){
							std::cout << "Vous avez choisi Burt" << std::endl;
							sortie=0;
							character.push_back(burt);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=406 && xClicked <=550 && yClicked>=338 && yClicked<=527){
							std::cout << "Vous avez choisi McKormack" << std::endl;
							sortie=0;
							character.push_back(mckormack);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);							
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=566 && xClicked <=710 && yClicked>=338 && yClicked<=527){
							std::cout << "Vous avez choisi Jennifer" << std::endl;
							sortie=0;
							character.push_back(jennifer);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							done=true;
						}
						if (xClicked>=68 && xClicked <=148 && yClicked>=38 && yClicked<=88){
							sortie=1;
							done = true;
						}
					}
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
				}
		}

		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if (d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}
	switch(sortie){
		case -1:
			break;
		case 0:
			sortie=menuCircuit(character,track);
			break;
		case 1:
			sortie=menuPrincipal(character,track);
			break;
	}
	
	glDeleteTextures(1,&idMenu);
	
	return sortie;
}

int menuPrincipal(std::vector<Character*>& character,std::vector<Track*>& track){
	int sortie=-1;
	VBO vbo;
	vbo.bind(GL_ARRAY_BUFFER);
	
	Vertex2DUV vertices[] = {
		Vertex2DUV(-1, -1, 0.0, 1.0),
		Vertex2DUV(-1, 1, 0.0, 0.0),
		Vertex2DUV(1, 1, 1.0, 0.0),
		Vertex2DUV(1, -1, 1.0, 1.0),
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

	Program prog;
	prog= loadProgram("../shaders/tex2D.vs.glsl", "../shaders/tex2D.fs.glsl");
	prog.use();

	GLint locVarTexture;
	locVarTexture= glGetUniformLocation(prog.getGLId(), "uTexture");

	//On loade l'image
	int img_width=0, img_height=0;
	unsigned char* img = SOIL_load_image("../textures/menuPrincipal800.jpg", &img_width, &img_height, NULL, 0);
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);

	bool done = false;
	while (!done) {
		Uint32 tStart = SDL_GetTicks();
		/* Rendering code goes here */
		//on nettoie la fenêtre
		glClear(GL_COLOR_BUFFER_BIT);
		//on rebinde le vao
		vao.bind();
		glUniform1i(locVarTexture,0);
		//Premier triangle
		glBindTexture(GL_TEXTURE_2D,idMenu);
		glUniform1i(locVarTexture,0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindTexture(GL_TEXTURE_2D,0);

		//on débinde le vao
		vao.debind();

		// Application code goes here

		SDL_Event e;
		int xClicked, yClicked;
		while (SDL_PollEvent(&e)) {

			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button==SDL_BUTTON_LEFT){
						xClicked=(float)(e.button.x);
						yClicked=(float)(e.button.y);
						if (xClicked>=438 && xClicked <=630 && yClicked>=229 && yClicked<=267){
							sortie=0;
							done = true;
						}
						else if (xClicked>=428 && xClicked <=642 && yClicked>=276 && yClicked<=314){
							sortie=1;
							done = true;
						}
						else if (xClicked>=470 && xClicked <=596 && yClicked>=324 && yClicked<=360){
							sortie=2;
							done = true;
						}
						else if (xClicked>=471 && xClicked <=598 && yClicked>=370 && yClicked<=410)
							sortie=-1;
							done = true;
					}
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
				}
		}

		// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
		SDL_GL_SwapBuffers();

		Uint32 tEnd = SDL_GetTicks();
		Uint32 d = tEnd - tStart;
		if (d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}

	glDeleteTextures(1,&idMenu);
	
	switch(sortie){
		case -1:
			break;
		case 0:
			sortie=menuPersonnage(character,track);
			break;
		case 1:
			sortie=lancerJeuRandom();
			break;
		case 2:
			std::cout << "Options" << std::endl;
			sortie=-1;
			break;
		default:
			break;
	}
	
	return sortie;
}

int lancerJeuRandom(){
	return -1;
}

std::vector<Kart*>& KartWithChar(std::vector<Character*>& Characters){
	std::vector<Kart*> Karts;
	
	for (std::vector<Character*>::iterator it = Characters.begin() ; it != Characters.end(); ++it){
		Kart* kart;
		
		switch((*it)->getHero()){
			case JOHN:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case KLAUS:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case DOUG:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case CANADA:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case BURT:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case MCKORMACK:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case STEVE:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case STAN:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case JENNIFER:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(0,0.5,0));
				kart->LoadObjFromFile("../models/ACC/ACC2.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
		}
		Karts.push_back(kart);
	}
}