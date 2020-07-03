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
	TransformComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject);
	TransformComponent(Json::Value& componentDetailsJSON);
	~TransformComponent();

	void update() override;

	virtual void setPosition(b2Vec2 position);
	virtual void setPosition(float xPosition, float yPosition);
	virtual void setPosition(b2Vec2 position, float angle);
	virtual void setPosition(float xPosition, float yPosition, float angle);
	virtual void setSize(b2Vec2 size);
	virtual void setSize(float xSize, float ySize);
	virtual void setAngle(float angle);

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

	float
		m_angle;
	b2Vec2
		m_position,
		m_size;
	bool
		m_absolutePositioning;


};




#endif