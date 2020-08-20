#include "ObjectPoolManager.h"

#include <fstream>
#include <chrono>
#include <json/json.h>

#include "GameObjectManager.h"
#include "GameObject.h"


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
		//lifetime = itr["lifetime"].asFloat();

		m_objectPool[poolId].reserve(maxItems);

		for (int index = 0; index < maxItems; index++) {

			//std::shared_ptr<GameObject> particle =
			//	std::make_shared<GameObject>(gameObjectId, -50, -50, 0);

			auto& gameObject = m_objectPool[poolId].emplace_back(std::make_shared<GameObject>(gameObjectId, -50.0f, -50.0f, 0.0f));
			gameObject->init(false);



			//particle->isAvailable = true;
			////convert seconds into miliseconds
			//particle->lifetime = particle->lifetimeRemaining = std::chrono::duration<float>(lifetime);
			//particle->poolId = poolId;
			//particle->setActive(false);



			//particle = GameObjectManager::instance().buildGameObject <ParticleObject>(gameObjectId, -50, -50, 0);
			//Game::instance().addGameObject(levelObject->gameObjectId, GameObjectLayer::MAIN, x, y, levelObject->angleAdjustment);

		
			//m_objectPool[poolId].push_back(particle);
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
		particleMap.second.clear();
	}

	this->m_objectPool.clear();
	
	
}

std::shared_ptr<GameObject> ObjectPoolManager::getParticle(std::string poolId)
{
	std::shared_ptr<GameObject>availParticle;

	for (auto particle : this->m_objectPool[poolId])
	{
		//convenience reference to outside component(s)
		const auto& particleComponent = particle->getComponent<ParticleComponent>();

		if (particleComponent->isAvailable() == true)
		{
			particleComponent->setAvailable(false);
			particleComponent->setActive(true);
			availParticle = particle;
			break;
		}
	}

	return (availParticle);

}

//void ObjectPoolManager::reset(std::shared_ptr<GameObject> particle)
//{
//	auto& particleComponent =
//		std::static_pointer_cast<ParticleComponent>(particle->components()[PARTICLE_COMPONENT]);
//
//	particle->setRemoveFromWorld(false);
//	particleComponent->reset();
//
//}
