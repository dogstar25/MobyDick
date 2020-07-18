#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <string>
#include <map>

#include "GameObjectDefinition.h"

class EntityDefinitionManager
{
public:
		
	static EntityDefinitionManager& instance();

	bool init();
	void load(std::string);
	
	std::shared_ptr<GameObjectDefinition> getDefinition(std::string gameObjectId);
	bool hasDefinition(std::string gameObjectId);


private:
	EntityDefinitionManager();
	~EntityDefinitionManager();

	//Map of the definitions of all posible game objects in the game/level
	std::map<std::string, GameObjectDefinition> m_gameObjectDefinitions;


	
};

#endif