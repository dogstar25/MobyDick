#ifndef ENTITY_DEFINITION_MANAGER_H
#define ENTITY_DEFINITION_MANAGER_H

#include <string>
#include <map>

#include "EntityDefinition.h"

class EntityDefinitionManager
{
public:
		
	static EntityDefinitionManager& instance();

	bool init();
	void load(std::string);
	
	std::shared_ptr<EntityDefinition> getDefinition(std::string gameObjectId);
	bool hasDefinition(std::string gameObjectId);


private:
	EntityDefinitionManager();
	~EntityDefinitionManager();

	//Map of the definitions of all posible game objects in the game/level
	std::map<std::string, EntityDefinition> m_entityDefinitions;


	
};

#endif