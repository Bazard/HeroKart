#pragma once

#include <vector>
#include <string>
#include "common.hpp"
#include <GL/glew.h>
#include "VBO.hpp"
#include "VAO.hpp"

class Object3D {

	GLuint idTexture;
	glimac::VAO vao;
	glimac::VBO vbo;
	
protected:
	glm::vec3 pos;
	glm::vec3 dir;
	float angle;
	
public:
	glm::mat4 MVMatrix;
	
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Object3D():m_nVertexCount(0), angle(0){dir.z=1;};

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
	void setPosition(glm::vec3 vec){ pos=vec;}
	void setDirection(glm::vec3 vec){ dir=vec;}
	
	float getAngle(){return angle;}
	
	void TransfoMatrix(glm::mat4 ViewMatrix, glm::vec3 tra, float angle, glm::vec3 sca);
	
	int LoadTexture();
	int LoadTexture(const char* sFilePath);
	
	int getIdTexture(){
		return idTexture;
	}
	
	std::vector<glimac::ShapeVertex>& getVertices(){
		return m_Vertices;
	}
	
	void Draw(GLuint uTex);
	
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<glimac::ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
