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
	this->m_gameObjectDefinitions.clear();
	std::map<std::string, GameObjectDefinition>().swap(this->m_gameObjectDefinitions);
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
	GameObjectDefinition* gameObjectDefinition;

	for (Json::Value itr : root["gameObjects"])
	{
		std::string gameObjectId = itr["id"].asString();

		m_gameObjectDefinitions.emplace(gameObjectId, *(new GameObjectDefinition(itr)));
	}
}



/*
	Retrieve the GameObjetc Definition
*/
std::shared_ptr<GameObjectDefinition> EntityDefinitionManager::getDefinition(std::string definitionId)
{

	if (m_gameObjectDefinitions.find(definitionId) == this->m_gameObjectDefinitions.end())
	{
		//FIXME: make it so that there at least 1  dummy definition in the map that it defaults to when nothign found
		//shoudl never happen
		return NULL;
	}
	else
	{
		return std::make_shared< GameObjectDefinition>(m_gameObjectDefinitions[definitionId]);
	}


}

bool EntityDefinitionManager::hasDefinition(std::string definitionId)
{

	if (m_gameObjectDefinitions.find(definitionId) == this->m_gameObjectDefinitions.end())
	{
		return false;
	}
	else
	{
		return true;
	}

}






