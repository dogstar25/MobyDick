#pragma once

#include <SDL.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <string>



using namespace std;

class LevelObject
{
public:
	string gameObjectId;
	int angleAdjustment;
};

class Level
{
public:

	string id; //probably same as the textureId since the map is represented by a texture
	int width, height;

	//2 dimentional array for the 2D map
	vector< vector <LevelObject> > levelObjects;

};

class LevelManager
{
public:

	map<string, Level*> levels;

	void loadLevel(string);
	LevelObject * determineTile(int,int,SDL_Surface*);

};

