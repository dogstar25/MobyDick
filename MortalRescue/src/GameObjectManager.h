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
	Animation * buildAnimation(GameObjectDefinition*, string, string, int, float);
	void load(string);

	//Map of the definitions of all posible game objects in the game/level
	map<string, GameObjectDefinition*> gameObjectDefinitions;

private:
	
};

