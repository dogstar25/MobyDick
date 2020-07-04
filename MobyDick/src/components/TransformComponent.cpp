#include "TransformComponent.h"
#include "../GameObjectDefinition.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject)
{
	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	if (itrJSON.isMember("transformComponent"))
	{
		Json::Value itrTransform = itrJSON["transformComponent"];

		m_parentGameObject->setComponentFlag(TRANSFORM_COMPONENT);

		m_position.SetZero();
		m_angle = 0;

		m_size.Set(itrTransform["size"]["width"].asFloat(), itrTransform["size"]["height"].asFloat());
		m_absolutePositioning = itrTransform["absolutePositioning"].asBool();

	}

}


TransformComponent::~TransformComponent()
{

}

void TransformComponent::update()
{

}

SDL_FRect TransformComponent::getPositionRect()
{
	SDL_FRect positionRect;

	positionRect.w = m_size.x;
	positionRect.h = m_size.y;

	positionRect.x = m_position.x;
	positionRect.y = m_position.y;

	return positionRect;

}

void TransformComponent::setPosition(b2Vec2 position)
{
	m_position = position;
}

void TransformComponent::setPosition(float xPosition, float yPosition)
{
	m_position.x = xPosition;
	m_position.x = yPosition;
}

void TransformComponent::setPosition(b2Vec2 position, float angle)
{
	m_position = position;
	m_angle = angle;
}

void TransformComponent::setPosition(float xPosition, float yPosition, float angle)
{
	m_position.x = xPosition;
	m_position.x = yPosition;
	m_angle = angle;
}

void TransformComponent::setSize(b2Vec2 size)
{
	m_size = size;
}

void TransformComponent::setSize(float xSize, float ySize)
{
	m_size.x = xSize;
	m_size.y = ySize;
}

void TransformComponent::setAngle(float angle)
{
	m_angle = angle;
}
