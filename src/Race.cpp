#include "Race.h"


Race::Race()
{
}


Race::~Race()
{
}

void Race::push_back(Track& track){
	Tracks.push_back(&track);
}