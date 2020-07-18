#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <box2d/box2d.h>

#include "Component.h"

class TransformComponent;

class PhysicsComponent : public Component
{

public:
	PhysicsComponent();
	PhysicsComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);
	~PhysicsComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;
	void applyMovement(float velocity, int direction, int strafeDirection);
	void applyRotation( float angularVelocity);
	void setOffGrid();
	void setTransform(b2Vec2 positionVector, float angle);
	void setLinearVelocity(b2Vec2 velocityVector);
	void setPhysicsBodyActive(bool  active);
	void attachItem(std::shared_ptr<GameObject>inventoryObject);

	//Accessor functions
	b2Vec2 objectAnchorPoint() {
		return m_objectAnchorPoint;
	}

private:

	b2Body* buildB2Body(Json::Value transformComponentJSON);
	uint16 setCollisionMask(uint16 category);

	b2Body* m_physicsBody;

	float
		m_friction,
		m_restitution,
		m_density,
		m_linearDamping,
		m_collisionRadius,
		m_angularDamping;
	uint16
		m_collisionCategory,
		m_collisionShape,
		m_physicsType;
	b2Vec2
		m_objectAnchorPoint;

};

#endif