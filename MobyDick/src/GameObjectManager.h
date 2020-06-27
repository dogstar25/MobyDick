#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <string>
#include <map>

#include "GameObjectDefinition.h"

class GameObjectManager
{
public:
		
	static GameObjectManager& instance();

	bool init();
	void load(std::string);
	
	GameObjectDefinition* getDefinition(std::string);

	//Map of the definitions of all posible game objects in the game/level
	std::map<std::string, GameObjectDefinition*> gameObjectDefinitions;
	
	/*
	Template function that builds any type of GameObject that you pass it
	Has to be inline because its a templated function
	*/
	template <typename gameObjectType>
	inline gameObjectType* buildGameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angle)
	{

		gameObjectType* gameObject = new gameObjectType(gameObjectId, xMapPos, yMapPos, angle);

		//string test = typeid(gameObject).name();
		return gameObject;
	}

private:
	GameObjectManager();
	~GameObjectManager();

	
};

#endif