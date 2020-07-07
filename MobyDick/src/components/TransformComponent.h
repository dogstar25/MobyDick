#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "box2d/box2d.h"
#include <SDL2/SDL.h>
#include <json/json.h>

#include "Component.h"

class TransformComponent : public Component
{

public:
	TransformComponent();
	TransformComponent(Json::Value definitionJSON, int xMapPos, int yMapPos, int angleAdjust);
	~TransformComponent();

	void update() override;

	void setPosition(b2Vec2 position);
	void setPosition(float xPosition, float yPosition);
	void setPosition(b2Vec2 position, float angle);
	void setPosition(float xPosition, float yPosition, float angle);
	void setSize(b2Vec2 size);
	void setSize(float xSize, float ySize);
	void setAngle(float angle);

	b2Vec2 size() {
		return m_size;
	}
	b2Vec2 position() {
		return m_position;
	}
	float angle() {
		return m_angle;
	}
	bool absolutePositioning() {
		return m_absolutePositioning;
	}

	virtual SDL_FRect  getPositionRect();

private:
	b2Vec2 calculatePosition(float xMapPos, float yMapPos, bool hasPhysicsComponent, Json::Value itrJSON);
	float calculateAngle(float angle, bool isPhysicsObject);
	float
		m_angle;
	b2Vec2
		m_position,
		m_size;
	bool
		m_absolutePositioning;


};




#endif