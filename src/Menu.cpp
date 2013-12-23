#include <iostream>
#include <cstdlib>
#include "VBO.hpp"
#include "VAO.hpp"
#include <vector>
#include "Program.hpp"
#include "Vertex2DUV.hpp"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include "Menu.h"

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

using namespace glimac;

void menuPrincipal(){
	
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
	unsigned char* img = SOIL_load_image("../textures/menuPrincipal.jpg", &img_width, &img_height, NULL, 0);
	//On créé la texture
	GLuint idMenu;
	glGenTextures(1, &idMenu);
	glBindTexture(GL_TEXTURE_2D, idMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	std::cout << "Le background est loade" << std::endl;

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
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			default:
				break;
			case SDL_QUIT:
				done = true;
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
}