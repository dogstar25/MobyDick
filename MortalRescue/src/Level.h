#pragma once
#include "Waypoint.h"
#include <string>
#include <vector>

struct LevelObject
{
	std::string gameObjectId;
	int angleAdjustment;
};

class Level
{
public:

	std::string id; //probably same as the textureId since the map is represented by a texture
	int width, height; // in tile count
	float tileWidth, tileHeight;

	std::vector< std::vector <LevelObject> > levelObjects;

	void addWaypoint(Waypoint waypoint);
	void addLevelObject(int xIndex, int yIndex, LevelObject levelObject);
	void setLevelObjectArraySize(int width, int height);

	//Accessor Functions
	std::vector<Waypoint> patrolWaypoints() {
		return m_patrolWaypoints;
	}

private:
	std::vector<Waypoint> m_patrolWaypoints;

};



