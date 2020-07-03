#include "VitalityComponent.h"

#include "../GameObjectManager.h"

VitalityComponent::VitalityComponent()
{

}


VitalityComponent::VitalityComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("VitalityComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

VitalityComponent::~VitalityComponent()
{

}

void VitalityComponent::update()
{

}




