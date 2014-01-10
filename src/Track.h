#pragma once
#include "PowerObject.h"
#include <vector>
#include <string>
#include "Node.h"

class Track
{
private:
	int nbLaps;
	int nbNodes;
	int largeur, longueur;
	std::vector<Object3D*> mapObjects; //Les objets qui composent ce circuit
	std::vector<PowerObject*> powObjects;
	std::string file; //Son fichier .map
	Node* nodeStart;
	
	std::vector<std::string> split(std::string str,std::string sep);
	
public:
	Track(std::string,int nbLaps);
	~Track();
	std::vector<Object3D*>& getMapObjects() {return mapObjects;}
	std::vector<PowerObject*>& getPowObjects() {return powObjects;}
	
	int getNbLaps(){ return nbLaps; };
	void setNbLaps(int l){ nbLaps = l; };

	int getNbNodes(){ return nbNodes; };
	void setNbNodes(int n){ nbNodes = n; };
	
	void push_back(Object3D& obj);
	void push_back_pow(PowerObject& pow);
	Node* getNodeStart(){return nodeStart;}
	void insertElt();
};

