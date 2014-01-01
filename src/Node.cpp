#include "Node.h"


Node::Node(int id, glm::vec3 position):id(id),position(position)
{
}

Node::Node(int id,float x,float y ,float z):id(id){
	position=glm::vec3(x,y,z);
}

Node::~Node()
{
}
