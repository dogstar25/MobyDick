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