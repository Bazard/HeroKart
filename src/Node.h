#pragma once
#include <glm/glm.hpp>

class Node
{
	glm::vec3 position;
public:
	
	int id;
	Node* next;
	Node(int,glm::vec3);
	Node(int,float,float,float);
	glm::vec3 getPosition();
	~Node();
	
};

