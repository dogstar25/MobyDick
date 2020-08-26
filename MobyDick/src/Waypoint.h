#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <string>
#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)


class Waypoint
{

public:
	std::string id;
	b2Vec2 point;
	

};

#endif