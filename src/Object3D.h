#pragma once
#include <vector>
#include <iostream>
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
	GLuint idTexture2;
	glimac::VAO vao;
	glimac::VBO vbo;
	
protected:
	glm::vec3 pos; //position
	glm::vec3 dir; //direction
	glm::vec3 sca; //scale
	float angle;
	glm::vec3 hitbox;
	
public:
	glm::mat4 MVMatrix;
	bool visible; //L'objet est-il visible ? 
	
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Object3D():m_nVertexCount(0), angle(0), visible(true), idTexture2(-1){dir.z=1; sca=glm::vec3(1,1,1); hitbox=glm::vec3(0,0,0);};
	
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
	glm::vec3 getHitbox(){ return hitbox;}

	void setPosition(glm::vec3 vec){ pos=vec;}
	void setPosition(float x,float y,float z){pos=glm::vec3(x,y,z);}
	
	void setDirection(glm::vec3 vec){
	dir=vec;
	if(dir.z==1){
			angle=dir.x*180/M_PI;
		}
		else
			angle=atan(-dir.x/(1-dir.z))*180/M_PI;
			
			if(dir.z<0){ 
					if(angle<0) angle+=180;
					else angle-=180;
				}
				while(angle>180) angle-=360;
				while(angle<-180) angle+=360;
				}
				
	void setDirection(float x,float y,float z){
	dir=glm::vec3(x,y,z);
	if(dir.z==1){
			angle=dir.x*180/M_PI;
		}
		else
			angle=atan(-dir.x/(1-dir.z))*180/M_PI;
			
			if(dir.z<0){ 
					if(angle<0) angle+=180;
					else angle-=180;
				}
				while(angle>180) angle-=360;
				while(angle<-180) angle+=360;
				}
	
	void setScale(glm::vec3 vec){ sca=vec;}

	void setHitbox(glm::vec3 size){ hitbox=size;}	
	
	bool isVisible(){ return visible;}
	float getAngle(){return angle;}
	void setAngle(float a){angle=a;}
	
	void TransfoMatrix(glm::mat4 ViewMatrix, glm::vec3 tra); //Transformation
	
	int LoadTexture();
	int LoadTexture(const char* sFilePath, int sec=0);
	
	int getIdTexture(){	return idTexture;}
	void setIdTexture(int i){idTexture=i;}
	int getIdTexture2(){return idTexture2;}
	void setIdTexture2(int i){idTexture2=i;}
	
	std::vector<glimac::ShapeVertex>& getVertices(){
		return m_Vertices;
	}
	
	virtual void movePower(){} //Fonction qui fait bouger l'objet, elle est utile seulement si l'objet est un powerObject
	virtual bool tooFar(){return false;}; //Fonction qui teste si l'objet est trop loin. Ne marche que pour les powerObject
	virtual bool isPerimed(int tStart){return false;} // Fonction qui teste si le powerObject doit se finir
	virtual int hitKart(Kart& kart, int id, int tStart){return 0;} //Fonction qui permet d'appliquer les pouvoirs du powerObjects
	virtual void hitKartBack(std::vector<Kart*>& karts){} //Fonction qui permet de finir le powerObject et d'annuler ses effets
	
	void Draw(GLuint uTex);
	void MatrixToShader(GLuint uMVMatrix,GLuint uMVPMatrix,GLuint uNormalMatrix, int, int);
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

	// Teste si l'objet entre en collision avec un autre objet
    bool isInCollision(Object3D &other);

    // Empêche les collisions entre objets
    void avoidCollision(Object3D &other);

};
