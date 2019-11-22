#pragma once
#include <stdio.h>
#include <string>
#include <map>

#include "TextObject.h"

#include "GameObjectDefinition.h"

//Forward declarations
class Animation;

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
	
	Animation* buildAnimation(GameObjectDefinition*, string, string, int, float);

	/*
	Template function that builds any type of GameObject that you pass it
	Has to be inline because its a templated function
	*/
	template <typename gameObjectType>
	inline gameObjectType* buildGameObject(string gameObjectId, int xMapPos, int yMapPos, int angle)
	{

		gameObjectType* gameObject = new gameObjectType(gameObjectId, xMapPos, yMapPos, angle);
		gameObject->buildChildren();


		//string test = typeid(gameObject).name();
		return gameObject;
	}

private:
	
};

