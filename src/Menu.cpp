#include <iostream>
#include <cstdlib>
#include <cmath>
#include "VBO.hpp"
#include "VAO.hpp"
#include <vector>
#include <ctime>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "Program.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include "Menu.h"

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_BPP = 32;

Mix_Music *musique;

using namespace glimac;

int redirection(std::vector<Character*>& character,std::vector<Track*>& track){
	int sortie=menuPrincipal(character, track);

	while(sortie!=-1 && sortie!=5){
		if (sortie==0){
			sortie=menuPrincipal(character,track);
		}
		else if (sortie==1){
			sortie=menuPersonnage(character,track);
		}
		else if(sortie==2){
			sortie=lancerJeuRandom(character,track);
		}
		else if(sortie==3){
			sortie=menuOptions();
		}
		else if(sortie==4){
			sortie=menuCircuit(character,track);
		}
	}
	
	if (sortie==5){
		menuChargement();
	}
	return sortie;
}

int redirectionPause(){
	int sortie=1;
	
	while(sortie!=2 && sortie!=-1){
		sortie=menuPause();
		if(sortie==1){
			sortie=menuOptions();
		}
	}
}

void menuChargement(){
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
	unsigned char* img;
	switch (WINDOW_WIDTH){
		case 1024:
			img=SOIL_load_image("../textures/chargement1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800:
			img=SOIL_load_image("../textures/chargement800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600:
			img=SOIL_load_image("../textures/chargement600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	
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


	// Mise à jour de la fenêtre (synchronisation implicite avec OpenGL)
	SDL_GL_SwapBuffers();

	glDeleteTextures(1,&idMenu);
	
}

int menuPause(){
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
	unsigned char* img;
	switch (WINDOW_WIDTH){
		case 1024:
			img=SOIL_load_image("../textures/menuCircuit1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800:
			img=SOIL_load_image("../textures/menuCircuit800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600:
			img=SOIL_load_image("../textures/menuCircuit600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
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
						if (xClicked>=(278.0/800.0)*WINDOW_WIDTH && xClicked <=(533.0/800.0)*WINDOW_WIDTH && yClicked>=(265.0/600.0)*WINDOW_HEIGHT && yClicked<=(286.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Reprise du jeu" << std::endl;
							sortie=6;
							done=true;
						}
						else if (xClicked>=(346.0/800.0)*WINDOW_WIDTH && xClicked <=(468.0/800.0)*WINDOW_WIDTH && yClicked>=(322.0/600.0)*WINDOW_HEIGHT && yClicked<=(342.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Options" << std::endl;
							sortie=7;
							done=true;
						}
						else if (xClicked>=(347.0/800.0)*WINDOW_WIDTH && xClicked <=(462.0/800.0)*WINDOW_WIDTH && yClicked>=(397.0/600.0)*WINDOW_HEIGHT && yClicked<=(416.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Quitter" << std::endl;
							exit(0);
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
	
	return sortie;
}

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
	unsigned char* img;
	switch (WINDOW_WIDTH){
		case 1024:
			img=SOIL_load_image("../textures/menuCircuit1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800:
			img=SOIL_load_image("../textures/menuCircuit800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600:
			img=SOIL_load_image("../textures/menuCircuit600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);

	Track* circuit1=new Track("../maps/Village.map",3);
	// circuit1->insertElt();
	Track* circuit2=new Track("../maps/Montreal.map",3);
	Track* circuit3=new Track("../maps/Village.map",3);
	
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
						if (xClicked>=(88.0/800.0)*WINDOW_WIDTH && xClicked <=(277.0/800.0)*WINDOW_WIDTH && yClicked>=(226.0/600.0)*WINDOW_HEIGHT && yClicked<=(416.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi le circuit 1" << std::endl;
							
							track.push_back(circuit1);
							track.push_back(circuit2);
							track.push_back(circuit3);
							sortie=5;
							done=true;
						}
						else if (xClicked>=(298.0/800.0)*WINDOW_WIDTH && xClicked <=(492.0/800.0)*WINDOW_WIDTH && yClicked>=(226.0/600.0)*WINDOW_HEIGHT && yClicked<=(416.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi le circuit 2" << std::endl;
							
							track.push_back(circuit2);
							track.push_back(circuit1);
							track.push_back(circuit3);
							sortie=5;
							done=true;
						}
						else if (xClicked>=(512.0/800.0)*WINDOW_WIDTH && xClicked <=(705.0/800.0)*WINDOW_WIDTH && yClicked>=(226.0/600.0)*WINDOW_HEIGHT && yClicked<=(416.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi le circuit 3" << std::endl;
							
							track.push_back(circuit3);
							track.push_back(circuit2);
							track.push_back(circuit1);
							sortie=5;
							done=true;
						}
						else if (xClicked>=(68.0/800.0)*WINDOW_WIDTH && xClicked<=(148.0/800.0)*WINDOW_WIDTH && yClicked>=(38.0/600.0)*WINDOW_HEIGHT && yClicked<=(88.0/600.0)*WINDOW_HEIGHT){
							for (std::vector<Track*>::iterator it = track.begin() ; it != track.end(); ++it)
								delete(*it);
							track.clear();
							
							for (std::vector<Character*>::iterator it = character.begin() ; it != character.end(); ++it)
								delete(*it);
							character.clear();
							
							sortie=1;
							done=true;
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
	
	return sortie;
}

int menuPersonnage(std::vector<Character*>& character, std::vector<Track*>& track){
	std::vector<int> konami;

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
	unsigned char* img;
	switch (WINDOW_WIDTH){
		case 1024:
			img=SOIL_load_image("../textures/menuPersonnage1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800:
			img=SOIL_load_image("../textures/menuPersonnage800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600:
			img=SOIL_load_image("../textures/menuPersonnage600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
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
		Character *john=new Character(JOHN,10000, "John");
		Character *klaus=new Character(KLAUS,10000, "Klaus");
		Character *doug=new Character(DOUG,10000, "Doug");
		Character *stan=new Character(STAN,10000, "Stan");
		Character *steve=new Character(STEVE,10000, "Steve");
		Character *burt=new Character(BURT,10000, "Burt");
		Character *mckormack=new Character(MCKORMACK,10000, "McKormack");
		Character *jennifer=new Character(JENNIFER,10000, "Jennifer");
		Character *canada=new Character(CANADA,10000, "Captain Canada");
		
		SDL_Event e;
		int xClicked, yClicked;
		while (SDL_PollEvent(&e)) {

			switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button==SDL_BUTTON_LEFT){
						xClicked=(float)(e.button.x);
						yClicked=(float)(e.button.y);
						if (xClicked>=(64.0/800.0)*WINDOW_WIDTH && xClicked <=(221.0/800.0)*WINDOW_WIDTH && yClicked>=(126.0/600.0)*WINDOW_HEIGHT && yClicked<=(340.0/600.0)*WINDOW_HEIGHT){
							sortie=4;
							std::cout << "Vous avez choisi John, bon choix" << std::endl;
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
						else if (xClicked>=(234.0/800.0)*WINDOW_WIDTH && xClicked <=(391.0/800.0)*WINDOW_WIDTH && yClicked>=(126.0/600.0)*WINDOW_HEIGHT && yClicked<=(340.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Klaus, bon choix" << std::endl;
							sortie=4;
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
						else if (xClicked>=(403.0/800.0)*WINDOW_WIDTH && xClicked <=(559.0/800.0)*WINDOW_WIDTH && yClicked>=(126.0/600.0)*WINDOW_HEIGHT && yClicked<=(340.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Doug, bon choix" << std::endl;
							sortie=4;
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
						else if (xClicked>=(572.0/800.0)*WINDOW_WIDTH && xClicked <=(728.0/800.0)*WINDOW_WIDTH && yClicked>=(126.0/600.0)*WINDOW_HEIGHT && yClicked<=(340.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Stan, bon choix" << std::endl;
							sortie=4;
							character.push_back(stan);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							character.push_back(jennifer);
							done=true;
						}
						else if (xClicked>=(64.0/800.0)*WINDOW_WIDTH && xClicked <=(221.0/800.0)*WINDOW_WIDTH && yClicked>=(346.0/600.0)*WINDOW_HEIGHT && yClicked<=(558.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Steve, bon choix" << std::endl;
							sortie=4;
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
						else if (xClicked>=(234.0/800.0)*WINDOW_WIDTH && xClicked <=(391.0/800.0)*WINDOW_WIDTH && yClicked>=(346.0/600.0)*WINDOW_HEIGHT && yClicked<=(558.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Burt, bon choix" << std::endl;
							sortie=4;
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
						else if (xClicked>=(403.0/800.0)*WINDOW_WIDTH && xClicked <=(559.0/800.0)*WINDOW_WIDTH && yClicked>=(346.0/600.0)*WINDOW_HEIGHT && yClicked<=(558.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi McKormack, bon choix" << std::endl;
							sortie=4;
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
						else if (xClicked>=(572.0/800.0)*WINDOW_WIDTH && xClicked <=(728.0/800.0)*WINDOW_WIDTH && yClicked>=(346.0/600.0)*WINDOW_HEIGHT && yClicked<=(558.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Vous avez choisi Jennifer, vous êtes pas dans la merde !" << std::endl;
							sortie=4;
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
						if (xClicked>=(68.0/800.0)*WINDOW_WIDTH && xClicked <=(148.0/800.0)*WINDOW_WIDTH && yClicked>=(38.0/600.0)*WINDOW_HEIGHT && yClicked<=(88.0/600.0)*WINDOW_HEIGHT){
							sortie=0;
							done = true;
						}
					}
					break;
				case SDL_KEYDOWN:
					if(konami.size()>=10)
						break;
					konami.push_back(e.key.keysym.sym);
					
					switch(konami.size()){
						case 1:
							if(konami[konami.size()-1]!=273) konami.clear();
							break;
						case 2:
							if(konami[konami.size()-1]!=273) konami.clear();
							break;
						case 3:
							if(konami[konami.size()-1]!=274) konami.clear();
							break;
						case 4:
							if(konami[konami.size()-1]!=274) konami.clear();
							break;
						case 5:
							if(konami[konami.size()-1]!=276) konami.clear();
							break;
						case 6:
							if(konami[konami.size()-1]!=275) konami.clear();
							break;
						case 7:
							if(konami[konami.size()-1]!=276) konami.clear();
							break;
						case 8:
							if(konami[konami.size()-1]!=275) konami.clear();
							break;
						case 9:
							if(konami[konami.size()-1]!=98) konami.clear();
							break;
						case 10:
							if(konami[konami.size()-1]!=113) konami.clear();
							else {
							std::cout << "Vous avez choisi Captain Canada, tabernacle, ca va chier !" << std::endl;
							sortie=4;
							character.push_back(canada);
							character.push_back(john);
							character.push_back(klaus);
							character.push_back(doug);
							character.push_back(stan);
							character.push_back(burt);
							character.push_back(steve);
							character.push_back(mckormack);
							done=true;
							}
							break;
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
	
	return sortie;
}

int menuOptions(){
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
	unsigned char* img;
	switch (WINDOW_WIDTH){
		case 1024:
			img=SOIL_load_image("../textures/menuOptions1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800:
			img=SOIL_load_image("../textures/menuOptions800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600:
			img=SOIL_load_image("../textures/menuOptions600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
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
		// glUniform1i(locVarTexture,0);
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
						if (xClicked>=(133.0/800.0)*WINDOW_WIDTH && xClicked <=(299.0/800.0)*WINDOW_WIDTH && yClicked>=(204.0/600.0)*WINDOW_HEIGHT && yClicked<=(351.0/600.0)*WINDOW_HEIGHT){
							std::cout << "1024 x 768" << std::endl;
							
							WINDOW_WIDTH=1024;
							WINDOW_HEIGHT=768;
							restartSDL();
							
							sortie=3;
							done=true;
							
						}
						else if (xClicked>=(337.0/800.0)*WINDOW_WIDTH  && xClicked <=(471.0/800.0)*WINDOW_WIDTH  && yClicked>=(227.0/600.0)*WINDOW_HEIGHT && yClicked<=(351.0/600.0)*WINDOW_HEIGHT){
							std::cout << "800 x 600" << std::endl;
							WINDOW_WIDTH=800;
							WINDOW_HEIGHT=600;
							restartSDL();
							sortie=3;
							done=true;
						}
						else if (xClicked>=(514.0/800.0)*WINDOW_WIDTH  && xClicked <=(628.0/800.0)*WINDOW_WIDTH  && yClicked>=(248.0/600.0)*WINDOW_HEIGHT && yClicked<=(351.0/600.0)*WINDOW_HEIGHT){
							std::cout << "600 x 450" << std::endl;
							WINDOW_WIDTH=600;
							WINDOW_HEIGHT=450;
							restartSDL();
							sortie=3;
							done=true;
						}
						else if (xClicked>=(151.0/800.0)*WINDOW_WIDTH  && xClicked <=(275.0/800.0)*WINDOW_WIDTH  && yClicked>=(445.0/600.0)*WINDOW_HEIGHT && yClicked<=(553.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Musique 1" << std::endl;
							musique = Mix_LoadMUS("../sounds/musique1.mp3");
							Mix_PlayMusic(musique, -1);
						}
						else if (xClicked>=(343.0/800.0)*WINDOW_WIDTH && xClicked <=(465.0/800.0)*WINDOW_WIDTH && yClicked>=(445.0/600.0)*WINDOW_HEIGHT && yClicked<=(553.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Musique 2" << std::endl;
							musique = Mix_LoadMUS("../sounds/musique2.mp3");
							Mix_PlayMusic(musique, -1);
						}
						else if (xClicked>=(512.0/800.0)*WINDOW_WIDTH && xClicked <=(635.0/800.0)*WINDOW_WIDTH && yClicked>=(445.0/600.0)*WINDOW_HEIGHT && yClicked<=(553.0/600.0)*WINDOW_HEIGHT){
							std::cout << "Musique 3" << std::endl;
							musique = Mix_LoadMUS("../sounds/musique3.mp3");
							Mix_PlayMusic(musique, -1);
						}
						if (xClicked>=(68.0/800.0)*WINDOW_WIDTH && xClicked <=(148.0/800.0)*WINDOW_WIDTH && yClicked>=(38.0/600.0)*WINDOW_HEIGHT && yClicked<=(88.0/600.0)*WINDOW_HEIGHT){
							sortie=0;
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
	unsigned char* img;
	switch(WINDOW_WIDTH){
		case 1024 :
			img=SOIL_load_image("../textures/menuPrincipal1024.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 800 :
			img=SOIL_load_image("../textures/menuPrincipal800.jpg", &img_width, &img_height, NULL, 0);
			break;
		case 600 :
			img=SOIL_load_image("../textures/menuPrincipal600.jpg", &img_width, &img_height, NULL, 0);
			break;
		default:
			break;
	}
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
						if (xClicked>=(418.0/800.0)*WINDOW_WIDTH && xClicked <=(649.0/800.0)*WINDOW_WIDTH && yClicked>=(225.0/600.0)*WINDOW_HEIGHT && yClicked<=(268.0/600.0)*WINDOW_HEIGHT){
							sortie=1;
							done = true;
						}
						else if (xClicked>=(402.0/800.0)*WINDOW_WIDTH && xClicked <=(666.0/800.0)*WINDOW_WIDTH && yClicked>=(276.0/600.0)*WINDOW_HEIGHT && yClicked<=(313.0/600.0)*WINDOW_HEIGHT){
							sortie=2;
							done = true;
						}
						else if (xClicked>=(459.0/800.0)*WINDOW_WIDTH && xClicked <=(610.0/800.0)*WINDOW_WIDTH && yClicked>=(321.0/600.0)*WINDOW_HEIGHT && yClicked<=(363.0/600.0)*WINDOW_HEIGHT){
							sortie=3;
							done = true;
						}
						else if (xClicked>=(459.0/800.0)*WINDOW_WIDTH && xClicked <=(610.0/800.0)*WINDOW_WIDTH && yClicked>=(371.0/600.0)*WINDOW_HEIGHT && yClicked<=(412.0/600.0)*WINDOW_HEIGHT){
							sortie=-1;
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
	
	
	return sortie;
}

int lancerJeuRandom(std::vector<Character*>& character,std::vector<Track*>& track){
	srand(time(NULL));
	int r=rand()%9;
	std::cout << r << std::endl;
	
		r=7;
	switch(r){
					case 0:
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 1:
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 2:
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 3:
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 4:
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 5:
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 6:
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							break;
					case 7:
							character.push_back(new Character(JENNIFER,10000, "Jennifer"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							break;
					case 8:
							
							character.push_back(new Character(CANADA,10000, "Captain Canada"));
							character.push_back(new Character(JOHN,10000, "John"));
							character.push_back(new Character(KLAUS,10000, "Klaus"));
							character.push_back(new Character(DOUG,10000, "Doug"));
							character.push_back(new Character(STAN,10000, "Stan"));
							character.push_back(new Character(BURT,10000, "Burt"));
							character.push_back(new Character(STEVE,10000, "Steve"));
							character.push_back(new Character(MCKORMACK,10000, "McKormack"));
							break;
					default:
							return -1;
							break;
					}
	
					r=rand()%4;
					
					Track* circuit1=new Track("../maps/Village.map",3);
					Track* circuit2=new Track("../maps/Montreal.map",3);
					Track* circuit3=new Track("../maps/Village.map",3);		
	
					switch(r){
						case 1:
							track.push_back(circuit1);
							track.push_back(circuit2);
							track.push_back(circuit3);
							break;
						case 2:
							track.push_back(circuit2);
							track.push_back(circuit1);
							track.push_back(circuit3);
							break;
						case 3:
							track.push_back(circuit3);
							track.push_back(circuit1);
							track.push_back(circuit2);
							break;
						default:
							return -1;
							break;
					}

	return 5;
}


void KartWithChar(std::vector<Character*>& Characters,std::vector<Kart*>& Karts){
	Karts.clear();
	
	for (std::vector<Character*>::iterator it = Characters.begin() ; it != Characters.end(); ++it){
		Kart* kart;
		
		switch((*it)->getHero()){
			case JOHN:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(-13,0,0));
				kart->setScale(glm::vec3(0.7));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.8));
				kart->LoadObjFromFile("../models/John.obj");	
				kart->LoadTexture("../textures/TexJohn.jpg");
				kart->LoadTexture("../textures/TexJohn2.jpg",1);
				kart->build();
				break;
			case KLAUS:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(-10,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/Klaus.obj");	
				kart->LoadTexture("../textures/TexKlaus.jpg");
				kart->build();
				break;
			case DOUG:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(-7,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.4, 0.5, 0.5));
				kart->LoadObjFromFile("../models/Doug.obj");	
				kart->LoadTexture("../textures/TexDoug.jpg");
				kart->build();
				break;
			case CANADA:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(-4,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/ACC.obj");	
				kart->LoadTexture("../textures/CCTex.jpg");
				kart->build();
				break;
			case BURT:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(-1,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/Burt.obj");	
				kart->LoadTexture("../textures/TexBurt.jpg");
				kart->build();
				break;
			case MCKORMACK:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(2,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/MacKormack.obj");	
				kart->LoadTexture("../textures/TexMac.jpg");
				kart->build();
				break;
			case STEVE:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(5,0,0));
				kart->setScale(glm::vec3(0.4));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.8));
				kart->LoadObjFromFile("../models/Steve.obj");	
				kart->LoadTexture("../textures/TexSteve.jpg");
				kart->build();
				break;
			case STAN:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(8,0,0));
				kart->setScale(glm::vec3(0.4));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/Stan.obj");	
				kart->LoadTexture("../textures/TexStan.jpg");
				kart->build();
				break;
			case JENNIFER:
				kart=new Kart(2,0.01,0.75,5);
				kart->setPosition(glm::vec3(11,0,0));
				kart->setScale(glm::vec3(0.5));
				kart->setHitbox(glm::vec3(0.5, 0.5, 0.6));
				kart->LoadObjFromFile("../models/Jennifer.obj");	
				kart->LoadTexture("../textures/TexJennifer.jpg");
				kart->build();
				break;
		}
		Karts.push_back(kart);
	}
}

void restartSDL(){
			Mix_FreeMusic(musique);
    		Mix_CloseAudio();
			SDL_Quit();
			SDL_Init(SDL_INIT_VIDEO);
			SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, SDL_OPENGL);
			SDL_WM_SetCaption("Hero Kaaaaart", NULL);
			glewInit();
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
			musique = Mix_LoadMUS("../sounds/musique3.mp3");
			Mix_PlayMusic(musique, -1);
}

GLuint* PowerTexture(){
	GLuint* tabTexture=new GLuint[7];
	// Power boost
	int img_width, img_height;
	unsigned char* img=SOIL_load_image("../textures/powerboost.jpg", &img_width, &img_height, NULL, 0);
	
	GLuint id1;
	glGenTextures(1, &id1);
	glBindTexture(GL_TEXTURE_2D, id1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[0]=id1;

	// Power front
	img=SOIL_load_image("../textures/powerfront.jpg", &img_width, &img_height, NULL, 0);

	GLuint id2;
	glGenTextures(1, &id2);
	glBindTexture(GL_TEXTURE_2D, id2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[1]=id2;

	// Power back
	img=SOIL_load_image("../textures/powerback.jpg", &img_width, &img_height, NULL, 0);

	GLuint id3;
	glGenTextures(1, &id3);
	glBindTexture(GL_TEXTURE_2D, id3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[2]=id3;

	// Power all
	img=SOIL_load_image("../textures/powerall.jpg", &img_width, &img_height, NULL, 0);

	GLuint id4;
	glGenTextures(1, &id4);
	glBindTexture(GL_TEXTURE_2D, id4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[3]=id4;

	// Power shield
	img=SOIL_load_image("../textures/powershield.jpg", &img_width, &img_height, NULL, 0);

	GLuint id5;
	glGenTextures(1, &id5);
	glBindTexture(GL_TEXTURE_2D, id5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[4]=id5;

	// Power trap
	img=SOIL_load_image("../textures/powertrap.jpg", &img_width, &img_height, NULL, 0);

	GLuint id6;
	glGenTextures(1, &id6);
	glBindTexture(GL_TEXTURE_2D, id6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[5]=id6;

	// Icone vide
	img=SOIL_load_image("../textures/vide.jpg", &img_width, &img_height, NULL, 0);

	GLuint id7;
	glGenTextures(1, &id7);
	glBindTexture(GL_TEXTURE_2D, id7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[6]=id7;

	return tabTexture;
}




GLuint* RankTexture(){
	GLuint* tabTexture=new GLuint[8];
	
	// 1er
	int img_width, img_height;
	unsigned char* img=SOIL_load_image("../textures/chiffres/1.jpg", &img_width, &img_height, NULL, 0);
	
	GLuint id1;
	glGenTextures(1, &id1);
	glBindTexture(GL_TEXTURE_2D, id1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[0]=id1;

	// 2ème
	img=SOIL_load_image("../textures/chiffres/2.jpg", &img_width, &img_height, NULL, 0);

	GLuint id2;
	glGenTextures(1, &id2);
	glBindTexture(GL_TEXTURE_2D, id2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[1]=id2;

	// 3ème
	img=SOIL_load_image("../textures/chiffres/3.jpg", &img_width, &img_height, NULL, 0);

	GLuint id3;
	glGenTextures(1, &id3);
	glBindTexture(GL_TEXTURE_2D, id3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[2]=id3;

	// 4ème
	img=SOIL_load_image("../textures/chiffres/4.jpg", &img_width, &img_height, NULL, 0);

	GLuint id4;
	glGenTextures(1, &id4);
	glBindTexture(GL_TEXTURE_2D, id4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[3]=id4;

	// 5ème
	img=SOIL_load_image("../textures/chiffres/5.jpg", &img_width, &img_height, NULL, 0);

	GLuint id5;
	glGenTextures(1, &id5);
	glBindTexture(GL_TEXTURE_2D, id5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[4]=id5;

	// 6ème
	img=SOIL_load_image("../textures/chiffres/6.jpg", &img_width, &img_height, NULL, 0);

	GLuint id6;
	glGenTextures(1, &id6);
	glBindTexture(GL_TEXTURE_2D, id6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[5]=id6;

	// 7ème
	img=SOIL_load_image("../textures/chiffres/7.jpg", &img_width, &img_height, NULL, 0);

	GLuint id7;
	glGenTextures(1, &id7);
	glBindTexture(GL_TEXTURE_2D, id7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[6]=id7;

	// 8ème
	img=SOIL_load_image("../textures/chiffres/8.jpg", &img_width, &img_height, NULL, 0);

	GLuint id8;
	glGenTextures(1, &id8);
	glBindTexture(GL_TEXTURE_2D, id8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[7]=id8;

	return tabTexture;
}


GLuint* PersoTexture(){
	GLuint* tabTexture=new GLuint[8];
	
	// John
	int img_width, img_height;
	unsigned char* img=SOIL_load_image("../textures/minipersos/john.jpg", &img_width, &img_height, NULL, 0);
	
	GLuint id1;
	glGenTextures(1, &id1);
	glBindTexture(GL_TEXTURE_2D, id1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[0]=id1;

	// Klaus
	img=SOIL_load_image("../textures/minipersos/klaus.jpg", &img_width, &img_height, NULL, 0);

	GLuint id2;
	glGenTextures(1, &id2);
	glBindTexture(GL_TEXTURE_2D, id2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[1]=id2;

	// Doug
	img=SOIL_load_image("../textures/minipersos/doug.jpg", &img_width, &img_height, NULL, 0);

	GLuint id3;
	glGenTextures(1, &id3);
	glBindTexture(GL_TEXTURE_2D, id3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[2]=id3;

	// Stan
	img=SOIL_load_image("../textures/minipersos/stan.jpg", &img_width, &img_height, NULL, 0);

	GLuint id4;
	glGenTextures(1, &id4);
	glBindTexture(GL_TEXTURE_2D, id4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[3]=id4;

	// Steve
	img=SOIL_load_image("../textures/minipersos/steve.jpg", &img_width, &img_height, NULL, 0);

	GLuint id5;
	glGenTextures(1, &id5);
	glBindTexture(GL_TEXTURE_2D, id5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[4]=id5;

	// Burt
	img=SOIL_load_image("../textures/minipersos/burt.jpg", &img_width, &img_height, NULL, 0);

	GLuint id6;
	glGenTextures(1, &id6);
	glBindTexture(GL_TEXTURE_2D, id6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[5]=id6;

	// McKormack
	img=SOIL_load_image("../textures/minipersos/mckormack.jpg", &img_width, &img_height, NULL, 0);

	GLuint id7;
	glGenTextures(1, &id7);
	glBindTexture(GL_TEXTURE_2D, id7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[6]=id7;

	// Jennifer
	img=SOIL_load_image("../textures/minipersos/jennifer.jpg", &img_width, &img_height, NULL, 0);

	GLuint id8;
	glGenTextures(1, &id8);
	glBindTexture(GL_TEXTURE_2D, id8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	tabTexture[7]=id8;

	return tabTexture;
}