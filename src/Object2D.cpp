#include "Object2D.h"
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

using namespace glimac;


Object2D::Object2D(){
}



void Object2D::build(){
    buildVBO();
    buildVAO();
}

void Object2D::buildVAO(){
    //on binde le vao
    vao.bind();
    //on remet le bateau au port
    vbo.bind(GL_ARRAY_BUFFER);
    //on attribue une position 2D qui aura pour id 0
    glEnableVertexAttribArray(0);
    //on définit les paramètres des attributs (position 2D)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, x)));
	//on attribue une texture qui aura pour id 1
    glEnableVertexAttribArray(1);
    //on définit les paramètres des attributs (textures)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, u)));

    vbo.debind();
    vao.debind();
}

void Object2D::buildVBO(){
    vbo.bind(GL_ARRAY_BUFFER);
    //on remplit les donnees du bateau
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2DUV)*4, &vertices[0], GL_STATIC_DRAW);
    //on remet le bateau à la mer
    vbo.debind();
}

VAO& Object2D::getVAO(){
    return vao;
}

VBO& Object2D::getVBO(){
    return vbo;
}

int Object2D::LoadTexture(){
    int img_width=0, img_height=0; 
    unsigned char* img = SOIL_load_image("../textures/default.png", &img_width, &img_height, NULL, 0);
    
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glBindTexture(GL_TEXTURE_2D,0);
    
    return idTexture;
}

int Object2D::LoadTexture(const char* sFilePath){
    int img_width=0, img_height=0;
    
    unsigned char* img = SOIL_load_image(sFilePath, &img_width, &img_height, NULL, 0);
    
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glBindTexture(GL_TEXTURE_2D,0);
      
    return idTexture;
}

int Object2D::getIdTexture(){
    return idTexture;
}

void Object2D::setVertices(float xdep, float xarr, float ydep, float yarr){
    // vertices.push_back(Vertex2DUV(xdep,ydep, 0.0, 1.0));
	// vertices.push_back(Vertex2DUV(xdep,yarr, 0.0, 0.0));
	// vertices.push_back(Vertex2DUV(xarr,yarr, 1.0, 0.0));
	// vertices.push_back(Vertex2DUV(xarr,ydep, 1.0, 1.0));
   vertices.clear();
		vertices.push_back(Vertex2DUV(0.7, -0.9, 0.0, 1.0));
		vertices.push_back(Vertex2DUV(0.7, -0.64, 0.0, 0.0));
		vertices.push_back(Vertex2DUV(0.9, -0.64, 1.0, 0.0));
		vertices.push_back(Vertex2DUV(0.9, -0.9, 1.0, 1.0));
}

std::vector<Vertex2DUV> Object2D::getVertices(){
    return vertices;
}

void Object2D::initDraw(GLint uTex){
    vao.bind();
    // glUniform1i(uTex,0);
}

void Object2D::bindTex(GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Object2D::Draw(GLint uTex){
    glUniform1i(uTex,0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	// glDrawArrays(GL_TRIANGLE_FAN, 1, 3);
    glBindTexture(GL_TEXTURE_2D,0);
}

