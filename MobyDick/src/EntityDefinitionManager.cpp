#include "EntityDefinitionManager.h"

#include <fstream>



EntityDefinitionManager& EntityDefinitionManager::instance()
{
	static EntityDefinitionManager singletonInstance;
	return singletonInstance;
}


EntityDefinitionManager::EntityDefinitionManager()
{

}

EntityDefinitionManager::~EntityDefinitionManager()
{

	//Use clear and swap to ensure free memory
	this->m_entityDefinitions.clear();
	std::map<std::string, EntityDefinition>().swap(this->m_entityDefinitions);
}


bool EntityDefinitionManager::init()
{
	load("gameObjectDefinitions/commonObjects2");
	load("levels/level1_Objects");
	load("gameObjectDefinitions/particleObjects");

	return true;
}

//GameObject GameObjectManager::buildGameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angle)
//{
//
//
//
//
//
//
//}



void EntityDefinitionManager::load(std::string gameObjectAssetsFilename)
{
	//Read file and stream it to a JSON object
	Json::Value root;
	std::string filename = "assets/" + gameObjectAssetsFilename + ".json";
	std::ifstream ifs(filename);
	ifs >> root;

	std::string id, textureId;
	EntityDefinition* entityDefinition;

	for (Json::Value itr : root["gameObjects"])
	{
		std::string gameObjectId = itr["id"].asString();

		m_entityDefinitions.emplace(gameObjectId, *(new EntityDefinition(itr)));
	}
}



/*
	Retrieve the GameObjetc Definition
*/
std::shared_ptr<EntityDefinition> EntityDefinitionManager::getDefinition(std::string definitionId)
{

	if (m_entityDefinitions.find(definitionId) == this->m_entityDefinitions.end())
	{
		//FIXME: make it so that there at least 1  dummy definition in the map that it defaults to when nothign found
		//shoudl never happen
		return NULL;
	}
	else
	{
		return std::make_shared< EntityDefinition>(m_entityDefinitions[definitionId]);
	}


}

bool EntityDefinitionManager::hasDefinition(std::string definitionId)
{

	if (m_entityDefinitions.find(definitionId) == this->m_entityDefinitions.end())
	{
		return false;
	}
	else
	{
		return true;
	}

}






