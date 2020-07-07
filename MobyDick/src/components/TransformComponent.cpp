#include "TransformComponent.h"
#include "../GameObjectDefinition.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"
#include "../GameConfig.h"


TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(Json::Value definitionJSON, int xMapPos, int yMapPos, int angleAdjust)
{
		Json::Value transformComponentJSON = definitionJSON["transformComponent"];

		m_gameObjectId = definitionJSON["id"].asString();;

		m_angle = angleAdjust;

		//FixMe: 32 is tilewidth
		setPosition( 
			(xMapPos * 32) + transformComponentJSON["size"]["width"].asFloat() / 2,
			(yMapPos * 32) + transformComponentJSON["size"]["height"].asFloat() / 2
		);

		//Size of both physics and non-physics objetcs needs to be set here
		//We cant easily get the size of the physics object later
		if (definitionJSON.isMember("physicsComponent") == true)
		{
			m_size.Set(
				transformComponentJSON["size"]["width"].asFloat() / GameConfig::instance().scaleFactor(),
				transformComponentJSON["size"]["height"].asFloat() / GameConfig::instance().scaleFactor());
		}
		else
		{
			m_size.Set(transformComponentJSON["size"]["width"].asFloat(), transformComponentJSON["size"]["height"].asFloat());
		}

		m_absolutePositioning = transformComponentJSON["absolutePositioning"].asBool();

}


TransformComponent::~TransformComponent()
{

}

b2Vec2 TransformComponent::calculatePosition(float xMapPos, float yMapPos, bool hasPhysicsComponent, Json::Value itrTransform)
{
	b2Vec2* position=nullptr;
	//Physics object requires center of the object
	if (hasPhysicsComponent)
	{
		position = new b2Vec2(
			xMapPos * 32 + (itrTransform["size"]["width"].asFloat() / 2) , 
			yMapPos * 32 + (itrTransform["size"]["width"].asFloat() / 2 ));
	}
	else
	{
		position = new b2Vec2(xMapPos * 32, yMapPos * 32);
	}

	return *position;
}

float TransformComponent::calculateAngle(float angle, bool isPhysicsObject)
{
	float newAngle = angle;

	if (isPhysicsObject)
	{
		newAngle = util::degreesToRadians(angle);
	}

	return newAngle;

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
	m_position.y = yPosition;
}

void TransformComponent::setPosition(b2Vec2 position, float angle)
{
	m_position = position;
	m_angle = angle;
}

void TransformComponent::setPosition(float xPosition, float yPosition, float angle)
{
	m_position.x = xPosition;
	m_position.y = yPosition;
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
