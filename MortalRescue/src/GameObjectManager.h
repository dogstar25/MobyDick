#pragma once
#include <stdio.h>
#include <string>
#include <map>

#include "GameObjectDefinition.h"

//Forward declarations
class GameObject;
class Animation;

extern Game* game;
using namespace std;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
		

	bool init();
	void load(string);
	GameObjectDefinition* getDefinition(string);

	//Map of the definitions of all posible game objects in the game/level
	map<string, GameObjectDefinition*> gameObjectDefinitions;
	
	//build animations and shit
	Animation* buildAnimation(GameObjectDefinition*, string, string, int, float);

private:
	
};

