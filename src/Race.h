#pragma once
#include "Track.h"
#include <vector>

class Race
{
private:
	std::vector<Track*> Tracks;
public:
	Race();
	~Race();
	std::vector<Track*>& getTracks(){ return Tracks;}
	void push_back(Track& track);
};

