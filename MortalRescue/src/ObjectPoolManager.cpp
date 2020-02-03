#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

#include "ObjectPoolManager.h"
#include "ParticleObject.h"


void ObjectPoolManager::init()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameObjectDefinitions/objectPools.json");
	ifs >> root;

	//Get and store config values
	string gameObjectId, poolId;
	ParticleObject* particle;
	int maxItems;
	float lifetime;
	duration<float, milli> lifetimeDur;


	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for (auto itr : root["gameObjectPools"])
	{

		poolId = itr["id"].asString();
		gameObjectId = itr["gameObjectId"].asString();
		maxItems = itr["maxItems"].asInt();
		lifetime = itr["lifetime"].asFloat();

		for (int index = 0; index < maxItems; index++) {
			particle = new ParticleObject(gameObjectId, -50, -50, 0);

			particle->isAvailable = true;
			//convert seconds into miliseconds
			particle->lifetime = particle->lifetimeRemaining = std::chrono::duration<float>(lifetime);
			particle->poolId = poolId;
			particle->physicsBody->SetActive(false);
			objectPool[poolId].push_back(particle);
		}

	}

}


ObjectPoolManager::ObjectPoolManager()
{

}


ObjectPoolManager::~ObjectPoolManager()
{
	for (auto particleMap : this->objectPool)
	{
		for (auto particle : particleMap.second)
		{
			delete particle;
		}
	}

	this-> objectPool.clear();
	
	
}

ParticleObject* ObjectPoolManager::get(string poolId)
{
	ParticleObject* availParticle=NULL;
	
	for (auto particle : this->objectPool[poolId])
	{
		if (particle->isAvailable == true)
		{
			particle->isAvailable = false;
			particle->physicsBody->SetActive(true);
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
	
	particle->removeFromWorld = false;
	particle->lifetimeRemaining = particle->lifetime;

	particle->physicsBody->SetTransform(positionVector, 0);
	particle->physicsBody->SetLinearVelocity(velocityVector);
	particle->physicsBody->SetActive(false);

	particle->isAvailable = true;



}
