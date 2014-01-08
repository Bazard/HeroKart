#include "Track.h"
#include <fstream>
#include <vector>

Track::Track(std::string s):file(s)
{
Node* node1=new Node(0,-30,2,-30);
Node* node2=new Node(0,-30,2,30);
Node* node3=new Node(0,30,2,30);
Node* node4=new Node(0,30,2,-30);

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
	
    while (!fichier.eof())
    {
		obj=new Object3D();
		
		obj->LoadObjFromFile(ligne);
		obj->build();
		std::cout << "Modele" << ligne << std::endl;
		
		std::getline(fichier, ligne);
		obj->LoadTexture((char*)ligne.c_str());
		std::cout << "Texture " << ligne << std::endl;
		
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setPosition((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str()));
		
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setDirection(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
		
		std::getline(fichier, ligne);
		fields=split(ligne," ");
		obj->setScale(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
		
		obj->visible=true;
		
        mapObjects.push_back(obj);
		std::getline(fichier, ligne);
		std::getline(fichier, ligne);
    }
	
	fichier.close();
 }

std::vector<std::string> Track::split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
		// std::cout << current << std::endl;
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}