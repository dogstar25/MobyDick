#include "TextComponent.h"

#include "../GameObjectManager.h"

TextComponent::TextComponent()
{

}

TextComponent::TextComponent(std::string gameObjectId)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Transform Component
	if (itrJSON.isMember("TextComponent"))
	{

		//m_componentFlags |= TRANSFORM_COMPONENT;

	}

}

TextComponent::~TextComponent()
{

}

void TextComponent::update()
{

}




