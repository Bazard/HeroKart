#include "Track.h"
#include <fstream>
#include <vector>
#include <string.h>

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
	
	while(ligne[0]=='#')
		std::getline(fichier, ligne);
		
		std::cout << ligne << std::endl;
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
			std::cout << "Modele " << ligne << std::endl;
			
			std::getline(fichier, ligne);
			pow->LoadTexture((char*)ligne.c_str());
			std::cout << "Texture " << ligne << std::endl;
			
			std::getline(fichier, ligne);
			fields=split(ligne," ");
			pow->setPosition((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str()));
			
			std::getline(fichier, ligne);
			fields=split(ligne," ");
			pow->setDirection(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
			
			std::getline(fichier, ligne);
			fields=split(ligne," ");
			pow->setScale(glm::vec3((float)atof(fields[0].c_str()),(float)atof(fields[1].c_str()),(float)atof(fields[2].c_str())));
			
			pow->visible=true;
			
			powObjects.push_back(pow);
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