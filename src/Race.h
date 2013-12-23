#pragma once
#include "Track.h"
#include <vector>

//Le championnat
class Race
{
private:
	//Composé de plusieurs courses
	std::vector<Track*> Tracks; 
public:
	Race();
	~Race();
	std::vector<Track*>& getTracks(){ return Tracks;} //Recuperer le vecteur de courses
	void push_back(Track& track); //Ajouter une course
};

