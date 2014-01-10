#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace glimac {

	class TrackballCamera {
		float m_fDistance; //Distance par rapport au centre de la scène
		float m_fAngleX; //angle selon x
		float m_fAngleY; //angle selon y
		
	public:
		TrackballCamera(): m_fDistance(2.5),m_fAngleX(0),m_fAngleY(180){};
		
		void moveFront(float delta){
			m_fDistance+=delta;
		}
		
		void rotateLeft(float degrees){
			m_fAngleX+=degrees;
		}
		
		void rotateUp(float degrees){
			m_fAngleY+=degrees;
		}
		
		glm::mat4 getViewMatrix(glm::vec3 pos, float angle, bool back) const {
			glm::mat4 VMatrix=glm::mat4(1);
			
			if(!back){
				VMatrix=glm::translate(VMatrix, glm::vec3(0,-1,-m_fDistance));
				VMatrix=glm::rotate(VMatrix, m_fAngleX, glm::vec3(1,0,0));
				VMatrix=glm::rotate(VMatrix, m_fAngleY-angle, glm::vec3(0,1,0));
				VMatrix=glm::translate(VMatrix,-glm::vec3(pos.x,pos.y,pos.z));
			}
			else {
				VMatrix=glm::translate(VMatrix, glm::vec3(0,-1,-m_fDistance));
				VMatrix=glm::rotate(VMatrix, m_fAngleX, glm::vec3(1,0,0));
				VMatrix=glm::rotate(VMatrix, m_fAngleY-angle-180, glm::vec3(0,1,0));
				VMatrix=glm::translate(VMatrix,-glm::vec3(pos.x,pos.y,pos.z));
			}
			
			return VMatrix;
		}
	};

}