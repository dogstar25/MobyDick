#include "ObjectPoolManager.h"


#include "game.h"


void ObjectPoolManager::init(Json::Value definitionJSON,  Scene* parentScene)
{

	Json::Value gameObjectPoolsJSON = definitionJSON["gameObjectPools"];

	//Get and store config values
	std::string gameObjectId, poolId;
	int maxItems;

	for (auto itr : gameObjectPoolsJSON)
	{

		poolId = itr["id"].asString();
		gameObjectId = itr["gameObjectId"].asString();
		maxItems = itr["maxItems"].asInt();

		m_objectPool[poolId].reserve(maxItems);

		for (int index = 0; index < maxItems; index++) {

			auto& gameObject = m_objectPool[poolId].emplace_back(std::make_shared<GameObject>(gameObjectId, -50.0f, -50.0f, 0.0f, parentScene));
			gameObject->reset();

			assert(gameObject->hasTrait(TraitTag::pooled) && "GameObject being pooled must be defined with the TraitTag::pooled trait");

		}

	}
}

ObjectPoolManager::ObjectPoolManager()
{



}

ObjectPoolManager::~ObjectPoolManager()
{
	clear();
	
	
}

std::optional<std::shared_ptr<GameObject>> ObjectPoolManager::getPooledObject(std::string poolId)
{
	for (auto& pooledObject : m_objectPool[poolId])
	{
		//convenience reference to outside component(s)
		const auto& poolComponent = pooledObject->getComponent<PoolComponent>(ComponentTypes::POOL_COMPONENT);

		if (poolComponent->isAvailable() == true)
		{
			const auto& vitalityComponent = pooledObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
			const auto& physicsComponent = pooledObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

			pooledObject->setRemoveFromWorld(false);

			poolComponent->setAvailable(false);
			physicsComponent->setPhysicsBodyActive(true);
			vitalityComponent->reset();
			return pooledObject;
		}
	}

	return std::nullopt;

}

std::shared_ptr<Texture> ObjectPoolManager::getPoolObjectTexture(std::string poolId)
{

	auto& gameObject = m_objectPool[poolId].at(0);
	auto texture = gameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->texture();

	return texture;
}

void ObjectPoolManager::clear()
{
	for (auto particleMap : m_objectPool)
	{
		particleMap.second.clear();
	}

	this->m_objectPool.clear();
}


//void ObjectPoolManager::reset(std::shared_ptr<GameObject> particle)
//{
//	auto& particleComponent =
//		std::static_pointer_cast<ParticleXComponent>(particle->components()[PARTICLE_COMPONENT]);
//
//	particle->setRemoveFromWorld(false);
//	particleComponent->reset();
//
//}
