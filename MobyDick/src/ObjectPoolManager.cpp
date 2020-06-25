#include "ObjectPoolManager.h"

#include <fstream>

#include <json/json.h>

#include "ParticleObject.h"
#include "GameObjectManager.h"


ObjectPoolManager& ObjectPoolManager::instance()
{
	static ObjectPoolManager singletonInstance;
	return singletonInstance;
}

void ObjectPoolManager::init()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	std::ifstream ifs("assets/gameObjectDefinitions/objectPools.json");
	ifs >> root;

	//Get and store config values
	std::string gameObjectId, poolId;
	ParticleObject* particle;
	int maxItems;
	float lifetime;
	std::chrono::duration<float, std::milli> lifetimeDur;


	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for (auto itr : root["gameObjectPools"])
	{

		poolId = itr["id"].asString();
		gameObjectId = itr["gameObjectId"].asString();
		maxItems = itr["maxItems"].asInt();
		lifetime = itr["lifetime"].asFloat();

		for (int index = 0; index < maxItems; index++) {
			particle = GameObjectManager::instance().buildGameObject <ParticleObject>(gameObjectId, -50, -50, 0);

			particle->isAvailable = true;
			//convert seconds into miliseconds
			particle->lifetime = particle->lifetimeRemaining = std::chrono::duration<float>(lifetime);
			particle->poolId = poolId;
			particle->setActive(false);
			m_objectPool[poolId].push_back(particle);
		}

	}

}


ObjectPoolManager::ObjectPoolManager()
{

}


ObjectPoolManager::~ObjectPoolManager()
{
	for (auto particleMap : m_objectPool)
	{
		for (auto particle : particleMap.second)
		{
			delete particle;
		}
	}

	this->m_objectPool.clear();
	
	
}

ParticleObject* ObjectPoolManager::getParticle(std::string poolId)
{
	ParticleObject* availParticle=NULL;
	
	for (auto particle : this->m_objectPool[poolId])
	{
		if (particle->isAvailable == true)
		{
			particle->isAvailable = false;
			particle->setActive(true);
			availParticle = particle;
			break;
		}
	}

	return (availParticle);

}

void ObjectPoolManager::reset(ParticleObject* particle)
{

	b2Vec2 velocityVector = b2Vec2(0, 0);
	b2Vec2 positionVector = b2Vec2(-50, -50);
	
	particle->setRemoveFromWorld(false);
	particle->lifetimeRemaining = particle->lifetime;

	particle->physicsBody()->SetTransform(positionVector, 0);
	particle->physicsBody()->SetLinearVelocity(velocityVector);
	particle->physicsBody()->SetActive(false);

	particle->isAvailable = true;



}
