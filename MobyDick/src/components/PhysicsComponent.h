#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <box2d/box2d.h>

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
};

#endif