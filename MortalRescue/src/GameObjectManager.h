#pragma once
#include <string>
#include <stdio.h>
#include <string>
#include <map>

//Forward declarations
class GameObjectDefinition;
class GameObject;
class GameObjectAnimation;

using namespace std;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
		

	bool init();
	b2Body* buildB2Body(GameObjectDefinition*);
	GameObjectAnimation * buildAnimation(GameObjectDefinition*, string, string, int, float);
	GameObject * buildGameObject(string , int, int, float=.0 );
	void load(string);

	//Map of the definitions of all posible game objects in the game/level
	map<string, GameObjectDefinition*> gameObjectDefinitions;
	int box2dBodyCount;

private:
	
};

