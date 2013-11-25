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
#include "Game.h"
#include "PlayerIA.h"
#include "Character.h"
#include "Kart.h"
#include "Race.h"
#include "Track.h"
#include "Vertex2DUV.hpp"
#include "Matrix2D.hpp"
#include <SOIL/SOIL.h>

#undef main

static const Uint32 FPS = 30;
static const Uint32 FRAME_DURATION = 1000.f / FPS;

static const Uint32 WINDOW_WIDTH = 800;
static const Uint32 WINDOW_HEIGHT = 600;
static const Uint32 WINDOW_BPP = 32;

#define M_PI 3.14

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

	VBO vbo;
	vbo.bind();

	std::vector<Vertex2DUV> vertices;
	
	//Triangle
	//vertices.push_back(Vertex3DRGB(0.0f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f));//Derriere
	vertices.push_back(Vertex2DUV(-1.0f, -1.f, 0.0f, 1.0f));//Gauche
	vertices.push_back(Vertex2DUV(1.0f, -1.0f, 1.0f, 1.0f));//Droite	
	vertices.push_back(Vertex2DUV(0.0f, 1.0f, 0.5f, 0.0f));	//Haut
	
	//Rectangle
	/*vertices.push_back(Vertex3DRGB(-1.0f, 1.0f, -0.5f, 1.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex3DRGB(-1.0f, -1.0f, -0.5f, 1.0f, 1.0f, 1.0f));	
	vertices.push_back(Vertex3DRGB(1.0f, 1.0f, -0.5f, 1.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex3DRGB(1.0f, -1.0f, -0.5f, 1.0f, 1.0f, 1.0f));*/

	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(Vertex2DUV), &vertices[0], GL_STATIC_DRAW);	
	VBO::debind(); // on débinde

	VAO vao;
	vao.bind();
	//Position
	glEnableVertexAttribArray(0);
	vbo.bind();
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(Vertex2DUV),(const GLvoid*) (0 * sizeof(GLfloat)));

	//Couleur
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(Vertex2DUV),(const GLvoid*) (2 * sizeof(GLfloat)));
	VBO::debind(); // on débinde

	VAO::debind();

	//setupColor2DShaders();
	Program prog;
	if(argc == 3){
		prog=loadProgram(argv[1],argv[2]);
		prog.use();
		}
	else{
		prog=loadProgram("../shaders/tex2D.vs.glsl","../shaders/tex2D.fs.glsl");
		prog.use();
	}
	
	//GLuint uTime=glGetUniformLocation(prog.getGLId(),"uTime");
	GLuint uModelMatrix=glGetUniformLocation(prog.getGLId(),"uModelMatrix");
	GLuint uColor=glGetUniformLocation(prog.getGLId(),"uColor");
	GLuint idTriforce=glGetUniformLocation(prog.getGLId(),"uTexture");
	
	Matrix2D matrotateorigin;
	Matrix2D matrotate;
	Matrix2D mattranslate;
	Matrix2D matscale;
	Matrix2D mattotal;
	
	bool done = false;
	float tourne=0;
	
	
	  int img_width=0, img_height=0;
  unsigned char* img = SOIL_load_image("../textures/triforce.png", &img_width, &img_height, NULL, 0);
	
	GLuint idTexture;
	glGenTextures(1,&idTexture);
	glBindTexture(GL_TEXTURE_2D,idTexture);
	

	  glGenTextures(1, &idTexture);
	  glBindTexture(GL_TEXTURE_2D, idTexture);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	
	
	while(!done) {
		
		Uint32 tStart = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT);
		vao.bind();
		
		tourne+=0.1;
		if(tourne>=180) tourne=0;
		
		matscale.scale(0.25,0.25);
		matrotateorigin.rotate(tourne);
		// Rendering code goes here
		
		//Triangle topleft
		matrotate.rotate(-3*tourne);
		mattranslate.translate(-2,2);
		mattotal=matrotateorigin*matscale*mattranslate*matrotate;
		glUniformMatrix3fv(uModelMatrix,9,false,mattotal.values);
	
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(idTriforce,0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//Triangle topright
		matrotate.rotate(tourne);
		mattranslate.translate(2,2);
		mattotal=matrotateorigin*matscale*mattranslate*matrotate;
		glUniformMatrix3fv(uModelMatrix,9,false,mattotal.values);
		
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(idTriforce,0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//Triangle bottomleft
		matrotate.rotate(tourne);
		mattranslate.translate(-2,-2);
		mattotal=matrotateorigin*matscale*mattranslate*matrotate;
		glUniformMatrix3fv(uModelMatrix,9,false,mattotal.values);
		
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(idTriforce,0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//Triangle bottomright
		matrotate.rotate(-3*tourne);
		mattranslate.translate(2,-2);
		mattotal=matrotateorigin*matscale*mattranslate*matrotate;
		glUniformMatrix3fv(uModelMatrix,9,false,mattotal.values);
		
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(idTriforce,0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//glLineWidth(20);
		VAO::debind();

		// Application code goes here

		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
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
		if(d < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - d);
		}
	}

	glDeleteTextures(1,&idTexture);
	
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