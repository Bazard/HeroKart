#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <cmath>
#include <iostream>

//static const float M_PI = 3.14159265358979323846;

namespace glimac {

	class FreeflyCamera {
		glm::vec3 m_Position; //Position de la caméra
		float m_fPhi;	//coordonnées sphérique du front vector
		float m_fTheta; //coordonnées sphérique du front vector
		glm::vec3 m_FrontVector; //Le front vector
		glm::vec3 m_LeftVector; //Le left pour calculer le Up
		glm::vec3 m_UpVector; // Le up
		
		void computeDirectionVectors(){
			m_FrontVector=glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
			m_LeftVector=glm::vec3(sin(m_fPhi+M_PI/2),0,cos(m_fPhi+M_PI/2));
			m_UpVector=glm::cross(m_FrontVector,m_LeftVector);
		}
		
	public:
		FreeflyCamera(){
			m_Position=glm::vec3(0,5,-5);
			m_fPhi=M_PI;
			m_fTheta=0;
			computeDirectionVectors();
		}
		
		void moveLeft(float t){
			m_Position += t*m_LeftVector;
		}
		
		void moveFront(float t){
			m_Position += t*m_FrontVector;
		}
		
		void rotateLeft(float degrees){
			m_fPhi+=M_PI*degrees/180;
			computeDirectionVectors();
		}
		
		void rotateUp(float degrees){
			m_fTheta+=M_PI*degrees/180;
			computeDirectionVectors();
		}
		
		glm::mat4 getViewMatrix() const {
			return glm::lookAt(m_Position,m_Position+m_FrontVector,m_UpVector);
		}
	};

}