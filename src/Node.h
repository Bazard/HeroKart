#pragma once
#include <glm/glm.hpp>

class Node
{
	glm::vec3 position;
	glm::vec3 leftPos;
	glm::vec3 rightPos;

public:
	
	int id;
	Node* next;
	Node(int,glm::vec3);
	Node(int,float,float,float);
	Node(int id, glm::vec3 leftPos, glm::vec3 rightPos);

	glm::vec3 getPosition();
	glm::vec3 getLeftPos();
	glm::vec3 getRightPos();

	~Node();
	
};

