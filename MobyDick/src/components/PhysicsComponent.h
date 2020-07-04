#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <box2d/box2d.h>

#include "Component.h"
class PhysicsComponent : public Component
{

public:
	PhysicsComponent();
	PhysicsComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject);
	~PhysicsComponent();

	void update() override;


private:

	b2Body* buildB2Body();
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


};

#endif