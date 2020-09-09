#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <SDL2/SDL.h>
#include <SFML/Graphics.hpp>

#include <box2d/box2d.h>
#include <json/json.h>


#include "Component.h"

class TransformComponent : public Component
{

public:
	TransformComponent();
	TransformComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);
	~TransformComponent();

	void update() override;

	void setSize(sf::Vector2f size);
	void setSize(float xSize, float ySize);
	void setAngle(float angle);

	b2Vec2 size() { return m_size; }
	b2Vec2 position() {	return m_position; }
	b2Vec2 originalPosition() {	return m_originalPosition; }
	float angle() {	return m_angle;	}
	bool absolutePositioning() { return m_absolutePositioning; }
	bool centeredPositioning() { return m_centeredPositioning; }

	SDL_FRect  getPositionRect();
	SDL_FPoint getCenterPosition();

private:
	b2Vec2
		m_size,
		m_originalPosition;
	bool
		m_absolutePositioning,
		m_centeredPositioning;;


};




#endif