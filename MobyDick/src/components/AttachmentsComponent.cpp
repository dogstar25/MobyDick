#include "AttachmentsComponent.h"

#include "../GameObjectManager.h"



AttachmentsComponent::AttachmentsComponent()
{
	//Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	//if (itrJSON.isMember("AttachmentsComponent"))
	//{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	//}

}

AttachmentsComponent::~AttachmentsComponent()
{

}

void AttachmentsComponent::update(std::shared_ptr<GameObject>gameObject)
{

}

