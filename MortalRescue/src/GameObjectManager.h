#pragma once
#include <stdio.h>
#include <string>
#include <map>

//Forward declarations
class GameObjectDefinition;
class GameObject;
class GameObjectAnimation;

extern Game* game;
using namespace std;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
		

	bool init();
	GameObjectAnimation * buildAnimation(GameObjectDefinition*, string, string, int, float);
	void load(string);

	//Map of the definitions of all posible game objects in the game/level
	map<string, GameObjectDefinition*> gameObjectDefinitions;
	int box2dBodyCount;

private:
	
};

