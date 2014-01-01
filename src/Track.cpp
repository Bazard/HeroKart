#include "Track.h"


Track::Track(std::string s):file(s)
{
Node* node1=new Node(0,-10,2,-10);
Node* node2=new Node(0,-10,2,10);
Node* node3=new Node(0,10,2,10);
Node* node4=new Node(0,10,2,-10);

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