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
	std::map<std::string, GameObjectDefinition> gameObjectDefinitions;
	
	GameObject buildGameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angle);

private:
	GameObjectManager();
	~GameObjectManager();

	//Map of the definitions of all posible game objects in the game/level
	std::map<std::string, GameObjectDefinition> m_gameObjectDefinitions;


	
};

#endif