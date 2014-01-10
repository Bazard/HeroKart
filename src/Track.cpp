#include "Track.h"
#include <fstream>
#include <vector>
#include <cstdio>
#include <string.h>


Track::Track(std::string s, int nbLaps):file(s),nbLaps(nbLaps)
{


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

void Track::insertElt(){
	// le constructeur de ifstream permet d'ouvrir un fichier en lecture
    std::ifstream fichier(file);
    int cpt=-1;
	std::string ligne;
	
    std::vector <std::string> fields;
	
    Object3D* obj;
	
	std::getline(fichier, ligne);
	
	while(ligne[0]=='#')
		std::getline(fichier, ligne);
	
	float angleNode;
	
    while (!fichier.eof())
    {
		if(ligne[0]=='#')
			break;
			
		obj=new Object3D();
		obj->LoadObjFromFile(ligne);
		obj->build();
		
		std::getline(fichier, ligne);
		obj->LoadTexture((char*)ligne.c_str());
		
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setPosition((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str()));
		
		std::getline(fichier, ligne);
		obj->setAngle((float)atof(ligne.c_str()));
	
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setScale(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
		
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setHitbox(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
		
		obj->visible=true;
		
        mapObjects.push_back(obj);
		std::getline(fichier, ligne);
		std::getline(fichier, ligne);
    }
	
	while(ligne[0]=='#')
		std::getline(fichier, ligne);
		
	PowerObject* pow;
	
	while (!fichier.eof()){
		if(ligne[0]=='#')
				break;
				
			if(ligne.compare("BOOST")==0)
					pow=new PowerObject(BOOST,10000);
			else if(ligne.compare("ATK_FRONT")==0)
					pow=new PowerObject(ATK_FRONT,10000);
			else if(ligne.compare("ATK_BACK")==0)
					pow=new PowerObject(ATK_BACK,10000);
			else if(ligne.compare("ATK_ALL")==0)
					pow=new PowerObject(ATK_ALL,10000);
			else if(ligne.compare("TRAP")==0)
					pow=new PowerObject(TRAP,10000);
			else if(ligne.compare("SHIELD")==0)
					pow=new PowerObject(SHIELD,10000);
			
			std::getline(fichier, ligne);
			pow->LoadObjFromFile(ligne);
			pow->build();
			
			std::getline(fichier, ligne);
			pow->LoadTexture((char*)ligne.c_str());
			
			std::getline(fichier, ligne);
			fields=split(ligne," ");
			pow->setPosition((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str()));
						
			std::getline(fichier, ligne);
			fields=split(ligne," ");
			pow->setScale(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
			
			pow->visible=true;
			
			powObjects.push_back(pow);
			std::getline(fichier, ligne);
			std::getline(fichier, ligne);
	}
	
	while(ligne[0]=='#')
		std::getline(fichier, ligne);
		
	Node* node;
	Node* oldNode=NULL;
	//Noeuds
	while (!fichier.eof()){
		fields=split(ligne," ");
		node=new Node((int)atoi(fields[0].c_str()),glm::vec3((float)atof(fields[1].c_str()),(float)atof(fields[2].c_str()),(float)atof(fields[3].c_str())),glm::vec3((float)atof(fields[4].c_str()),(float)atof(fields[5].c_str()),(float)atof(fields[6].c_str())));
		if(oldNode)
			oldNode->next=node;
		else
			nodeStart=node;
		oldNode=node;
		std::getline(fichier, ligne);
	}
	node->next=nodeStart;
	nbNodes=node->id+1;
	fichier.close();
 }

std::vector<std::string> Track::split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}