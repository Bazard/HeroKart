#include "Node.h"


Node::Node(int id, glm::vec3 position):id(id),position(position)
{
}

Node::Node(int id,float x,float y ,float z):id(id){
	position=glm::vec3(x,y,z);
}

Node::Node(int id, glm::vec3 leftPos, glm::vec3 rightPos) : 
	id(id), leftPos(leftPos), rightPos(rightPos){
	// Calcule la position du milieu du segment pour avoir la position du noeud pour l'IA
	position=glm::vec3( (rightPos.x+leftPos.x)/2 , (rightPos.y+leftPos.y)/2 , (rightPos.z+leftPos.z)/2 );
}

glm::vec3 Node::getPosition(){
	return position;
}

glm::vec3 Node::getLeftPos(){
	return leftPos;
}

glm::vec3 Node::getRightPos(){
	return rightPos;
}

Node::~Node()
{
}
