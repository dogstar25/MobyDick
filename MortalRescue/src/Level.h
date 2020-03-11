#pragma once
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

	//2 dimentional array for the 2D map
	std::vector< std::vector <LevelObject> > levelObjects;

};



