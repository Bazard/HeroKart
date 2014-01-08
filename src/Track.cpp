#include "Track.h"


Track::Track(std::string s):file(s)
{
/*Node* node1=new Node(0,-30,2,-30);
Node* node2=new Node(1,-30,2,30);
Node* node3=new Node(2,30,2,30);
Node* node4=new Node(3,30,2,-30);*/

Node* node1=new Node(0,glm::vec3(-35,2,-35),glm::vec3(-25,2,-25));
Node* node2=new Node(1,glm::vec3(-35,2,25),glm::vec3(-25,2,35));
Node* node3=new Node(2,glm::vec3(25,2,25),glm::vec3(35,2,35));
Node* node4=new Node(3,glm::vec3(25,2,-35),glm::vec3(35,2,-25));

nodeStart=node1;
node1->next=node2;
node2->next=node3;
node3->next=node4;
node4->next=node1;
}


Track::~Track()
{
}

void Track::push_back(Object3D& obj){
	mapObjects.push_back(&obj);
}

void Track::push_back_pow(PowerObject& pow){
	powObjects.push_back(&pow);
}