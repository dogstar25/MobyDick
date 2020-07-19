#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "box2d/box2d.h"
#include "json/json.h"

struct TransformComponent
{
	float m_angle;
	b2Vec2 m_position;
	b2Vec2 m_size;
	b2Vec2 m_originalPosition;
	bool m_absolutePositioning;

	TransformComponent();
	TransformComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);

	SDL_FRect getPositionRect();
};

#endif