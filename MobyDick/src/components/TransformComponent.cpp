#include "TransformComponent.h"
#include "../Game.h"

extern std::unique_ptr<Game> game;

TransformComponent::TransformComponent()
{
}

TransformComponent::TransformComponent(Json::Value componentJSON, float xMapPos, float yMapPos, float angleAdjust)
{
		m_angle = angleAdjust;

		auto objectWidth = componentJSON["size"]["width"].asFloat();
		auto objectHeight = componentJSON["size"]["height"].asFloat();

		m_centeredPositioning = componentJSON["centeredPositioning"].asBool();

		if (m_centeredPositioning == true) {

			setPosition(
				((xMapPos * game->worldTileWidth()) + objectWidth / 2) - (objectWidth  / 2),
				((yMapPos * game->worldTileHeight()) + objectHeight / 2) - (objectHeight / 2)
			);
		}
		else {

			setPosition(
				(xMapPos * game->worldTileWidth()) + objectWidth / 2,
				(yMapPos * game->worldTileHeight()) + objectHeight / 2
			);
		}

		m_originalPosition = m_position;
		m_size.Set(componentJSON["size"]["width"].asFloat(), componentJSON["size"]["height"].asFloat());

		m_absolutePositioning = componentJSON["absolutePositioning"].asBool();
		

}


TransformComponent::~TransformComponent()
{

}


void TransformComponent::update()
{


}

SDL_FRect TransformComponent::getPositionRect()
{
	SDL_FRect positionRect{};
	SDL_FPoint position{};

	positionRect.w = m_size.x;
	positionRect.h = m_size.y;

	//Adjust to make it top left
	position = getTopLeftPosition();
	positionRect.x = position.x;
	positionRect.y = position.y;

	return positionRect;

}

SDL_FPoint TransformComponent::getCenterPosition()
{
	return SDL_FPoint{ m_position.x, m_position.y };

}

SDL_FPoint TransformComponent::getTopLeftPosition()
{
	SDL_FPoint position{};

	position.x = m_position.x - (m_size.x / 2);
	position.y = m_position.y - (m_size.y / 2);

	return position;
}

void TransformComponent::setPosition(b2Vec2 position)
{
	m_position = position;
}

void TransformComponent::setPosition(SDL_FPoint position)
{
	m_position.x = position.x;
	m_position.y = position.y;
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

void TransformComponent::setLine(b2Vec2 start, b2Vec2 end)
{
	m_lineStart = start;
	m_lineEnd = end;
}

void TransformComponent::setAbsolutePositioning(bool absolutePositioning)
{
	m_absolutePositioning = absolutePositioning;
}

