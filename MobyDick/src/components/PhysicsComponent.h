#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <box2d/box2d.h>
#include <json/json.h>

struct PhysicsComponent
{
	b2Body* m_physicsBody;
	float m_friction;
	float m_restitution;
	float m_density;
	float m_linearDamping;
	float m_collisionRadius;
	float m_angularDamping;
	uint16 m_collisionCategory;
	uint16 m_collisionShape;
	uint16 m_physicsType;
	b2Vec2 m_objectAnchorPoint;

	PhysicsComponent();
	PhysicsComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);
	void setTransform(b2Vec2 positionVector, float angle);
	void setPhysicsBodyActive(bool  active);
	void setLinearVelocity(b2Vec2 velocityVector);
	void applyMovement(float velocity, int direction, int strafeDirection);
	void applyRotation(float angularVelocity);
	void setOffGrid();
	void update();

private:
	b2Body* _buildB2Body(Json::Value definitionJSON);
	uint16 _setCollisionMask(uint16 category);

};

#endif