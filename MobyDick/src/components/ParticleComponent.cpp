#include "ParticleComponent.h"

#include "../GameObjectManager.h"

ParticleComponent::ParticleComponent()
{

}

ParticleComponent::ParticleComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("ParticleComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

ParticleComponent::~ParticleComponent()
{

}

void ParticleComponent::update()
{

}




