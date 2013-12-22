#pragma once

#include <vector>
#include <string>
#include "common.hpp"
#include <GL/glew.h>
#include "VBO.hpp"
#include "VAO.hpp"

class Kart;
class Object3D {

	std::vector<glimac::ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
	GLuint idTexture;
	glimac::VAO vao;
	glimac::VBO vbo;
	
protected:
	glm::vec3 pos; //position
	glm::vec3 dir; //direction
	glm::vec3 sca; //scale
	float angle;
	
public:
	glm::mat4 MVMatrix;
	bool visible; //L'objet est-il visible ? 
	int id; //S'il est contenu dans un tableau
	
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Object3D():m_nVertexCount(0), angle(0),visible(true){dir.z=1; sca=glm::vec3(1,1,1);};
	
	Object3D(const Object3D&);
	
	bool LoadObjFromFile(std::string sFilePath);
    // Renvoit le pointeur vers les données
    const glimac::ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }
    void sphere(GLfloat radius, GLsizei discLat, GLsizei discLong);
	void cone(GLfloat height, GLfloat r, GLsizei discLat, GLsizei discHeight);
	void build();
	void buildVAO();
	void buildVBO();
	glimac::VAO& getVAO();
	glimac::VBO& getVBO();

	glm::vec3 getPosition(){ return pos;}
	glm::vec3 getDirection(){ return dir;}
	glm::vec3 getScale(){ return sca;}
	void setPosition(glm::vec3 vec){ pos=vec;}
	void setDirection(glm::vec3 vec){ dir=vec;}
	void setScale(glm::vec3 vec){ sca=vec;}
	
	bool isVisible(){ return visible;}
	float getAngle(){return angle;}
	
	void TransfoMatrix(glm::mat4 ViewMatrix, float angle, glm::vec3 tra); //Transformation
	
	int LoadTexture();
	int LoadTexture(const char* sFilePath);
	
	int getIdTexture(){
		return idTexture;
	}
	
	std::vector<glimac::ShapeVertex>& getVertices(){
		return m_Vertices;
	}
	
	void Draw(GLuint uTex);
	void MatrixToShader(GLuint uMVMatrix,GLuint uMVPMatrix,GLuint uNormalMatrix, int WINDOW_WIDTH, int WINDOW_HEIGHT);
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

};
