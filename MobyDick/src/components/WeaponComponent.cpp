#include "WeaponComponent.h"

#include "../GameObjectManager.h"

WeaponComponent::WeaponComponent()
{

}

WeaponComponent::WeaponComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("WeaponComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

WeaponComponent::~WeaponComponent()
{

}

void WeaponComponent::update()
{

}
