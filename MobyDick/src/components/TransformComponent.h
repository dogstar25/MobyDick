#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <SDL2/SDL.h>

#pragma warning(push,0)
#include <box2d/box2d.h>
#include <json/json.h>
#pragma warning(pop)

#include "Component.h"

class TransformComponent : public Component
{

public:
	TransformComponent();
	TransformComponent(Json::Value componentJSON, float xMapPos, float yMapPos, float angleAdjust);
	~TransformComponent();

	void update() override;

	void setPosition(b2Vec2 position);
	void setPosition(SDL_FPoint position);
	void setPosition(float xPosition, float yPosition);
	void setPosition(b2Vec2 position, float angle);
	void setPosition(float xPosition, float yPosition, float angle);
	void setSize(b2Vec2 size);
	void setSize(float xSize, float ySize);
	void setAngle(float angle);
	void setLine(b2Vec2 start, b2Vec2 end);
	void setAbsolutePositioning(bool absolutePositioning);

	b2Vec2 size() { return m_size; }
	b2Vec2 position() {	return m_position; }
	b2Vec2 originalPosition() {	return m_originalPosition; }
	b2Vec2 originalTilePosition() { return m_originalTilePosition; }
	float originalAngle() { return m_originalAngle; }
	float angle() {	return m_angle;	}
	bool absolutePositioning() { return m_absolutePositioning; }
	bool centeredPositioning() { return m_centeredPositioning; }
	b2Vec2 lineStart() { return m_lineStart; }
	b2Vec2 lineEnd() { return m_lineEnd; }

	SDL_FRect  getPositionRect();
	SDL_FPoint getCenterPosition();
	SDL_FPoint getTopLeftPosition();

private:
	float
		m_angle,
		m_originalAngle{};
	b2Vec2
		m_position,
		m_size,
		m_originalTilePosition,
		m_originalPosition;

	bool
		m_absolutePositioning,
		m_centeredPositioning;

	//Primitive Point
	b2Vec2 m_point;

	//Primitive Line
	b2Vec2
		m_lineStart,
		m_lineEnd;

};




#endif