#include "TransformComponent.h"
#include "../GameObjectDefinition.h"
#include "../GameObjectManager.h"
#include "../GameObject.h"
#include "../GameConfig.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(Json::Value itrJSON, float xMapPos, float yMapPos, float angleAdjust)
{
		Json::Value itrTransform = itrJSON["transformComponent"];

		/*
		If this is a physics object, then this all will get overridden current he first 
		Game loop on pyhsicsComponent.Update()
		*/
		m_angle = angleAdjust;

		//FixMe: 32 is tilewidth
		setPosition( 
			(xMapPos * 32) + itrTransform["size"]["width"].asFloat() / 2, 
			(yMapPos * 32) + itrTransform["size"]["height"].asFloat() / 2
		);

		//Size of both physics and non-physics objetcs needs to be set here
		//We cant easily get the size of the physics object later
		if (itrJSON.isMember("physicsComponent") == true)
		{
			m_size.Set(
				itrTransform["size"]["width"].asFloat() / GameConfig::instance().scaleFactor(), 
				itrTransform["size"]["height"].asFloat() / GameConfig::instance().scaleFactor());
		}
		else
		{
			m_size.Set(itrTransform["size"]["width"].asFloat(), itrTransform["size"]["height"].asFloat());
		}

		m_absolutePositioning = itrTransform["absolutePositioning"].asBool();

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
