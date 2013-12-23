#pragma once
#include "Object3D.h"
#include <vector>
#include <string>

class Track
{
private:
	int nblaps;
	std::vector<Object3D*> mapObjects; //Les objets qui composent ce circuit
	std::string file; //Son fichier .map
	
public:
	Track(std::string);
	~Track();
	std::vector<Object3D*>& getMapObjects() {return mapObjects;}
	void push_back(Object3D& obj);
};

