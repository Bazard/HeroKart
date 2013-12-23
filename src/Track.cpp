#include "Track.h"


Track::Track(std::string s):file(s)
{
}


Track::~Track()
{
}

void Track::push_back(Object3D& obj){
	mapObjects.push_back(&obj);
}