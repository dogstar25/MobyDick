#include "PhysicsComponent.h"

#include "../GameObjectManager.h"

PhysicsComponent::PhysicsComponent()
{

}

PhysicsComponent::PhysicsComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("PhysicsComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::update()
{

}


