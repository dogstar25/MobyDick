#include "CompositeComponent.h"

#include "../GameObjectManager.h"


CompositeComponent::CompositeComponent()
{
	//Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	////Transform Component
	//if (itrJSON.isMember("CompositeComponent"))
	//{

	//	//m_componentFlags |= TRANSFORM_COMPONENT;

	//}

}

CompositeComponent::~CompositeComponent()
{

}
void CompositeComponent::update()
{

}

