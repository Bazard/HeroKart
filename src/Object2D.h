#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "common.hpp"
#include <GL/glew.h>
#include "VBO.hpp"
#include "VAO.hpp"
#include "Vertex2DUV.hpp"


class Object2D {

	std::vector<Vertex2DUV> vertices;
	GLuint idTexture;
	glimac::VAO vao;
	glimac::VBO vbo;	
	
public:
    Object2D();
	/*Object2D(const Object2D&);*/
	void build();
	void buildVAO();
	void buildVBO();
	glimac::VAO& getVAO();
	glimac::VBO& getVBO();
	int LoadTexture();
	int LoadTexture(const char* sFilePath);	
	int getIdTexture();	
	void setVertices(float xdep, float xarr, float ydep, float yarr);
	std::vector<Vertex2DUV> getVertices();
	void initDraw(GLint uTex);
	void bindTex(GLuint texture);
	void Draw(GLint uTex);
};
