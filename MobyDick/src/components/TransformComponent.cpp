#include "TransformComponent.h"

#include "json/json.h"

TransformComponent::TransformComponent(Json::Value& componentDetailsJSON)
{
	//Convenience reference
	Json::Value itr = componentDetailsJSON;

	m_position.SetZero();
	m_angle = 0;

	m_size.Set(itr["size"]["width"].asFloat(), itr["size"]["height"].asFloat());
	m_absolutePositioning = itr["absolutePositioning"].asBool();
	

}

TransformComponent::~TransformComponent()
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
