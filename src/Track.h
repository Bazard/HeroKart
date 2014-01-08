#pragma once
#include "Object3D.h"
#include "PowerObject.h"
#include <vector>
#include <string>
#include "Node.h"

class Track
{
private:
	int nblaps;
	int nbNodes;
	std::vector<Object3D*> mapObjects; //Les objets qui composent ce circuit
	std::vector<PowerObject*> powObjects;
	std::string file; //Son fichier .map
	Node* nodeStart;
	
public:
	Track(std::string);
	~Track();
	std::vector<Object3D*>& getMapObjects() {return mapObjects;}
	std::vector<PowerObject*>& getPowObjects() {return powObjects;}
	
	void push_back(Object3D& obj);
	void push_back_pow(PowerObject& pow);
	Node* getNodeStart(){return nodeStart;}
};

