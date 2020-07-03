#include "AttachmentsComponent.h"

#include "../GameObjectManager.h"

AttachmentsComponent::AttachmentsComponent()
{

}

AttachmentsComponent::AttachmentsComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("AttachmentsComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

AttachmentsComponent::~AttachmentsComponent()
{

}

void AttachmentsComponent::update()
{

}

