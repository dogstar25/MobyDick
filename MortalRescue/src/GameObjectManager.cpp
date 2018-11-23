#include "GameObjectManager.h"
#include "TextureManager.h"


bool GameObjectManager::init(TextureManager* textureManager )
{


	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameObjectAssets.json");
	ifs >> root;

	string id, textureId;
	GameObject* gameObject;

	for (auto itr : root["gameObjects"])
	{
		gameObject = new GameObject();
		gameObject->id = itr["id"].asString();
		gameObject->description = itr["description"].asString();
		gameObject->isStaticObject = itr["static"].asBool();
		gameObject->isAnimated = itr["animated"].asBool();
		gameObject->speed = itr["speed"].asInt();
		
		
		//If this is not an animated object then store its one texture
		if (gameObject->isAnimated == false)
		{
			textureId = itr["texture"].asString();
			gameObject->staticTexture = textureManager->getTexture(textureId);
			gameObject->xSize = itr["xSize"].asInt();
			gameObject->ySize = itr["ySize"].asInt();
		}
		else
		{
			for (auto animItr : itr["annimations"])
			{



			}


		}
		


		this->gameObjectMap[gameObject->id] = *gameObject;


	}

	return true;

}

GameObject * GameObjectManager::getGameObject(string id)
{
	GameObject *gameObject = nullptr;

	gameObject = &this->gameObjectMap[id];

	return gameObject;
}


GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}
