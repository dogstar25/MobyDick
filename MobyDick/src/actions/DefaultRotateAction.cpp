#include "DefaultRotateAction.h"


#include <iostream>

#include "../GameObject.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"



DefaultRotateAction::DefaultRotateAction() :
	RotateAction(0)
{

}

DefaultRotateAction::DefaultRotateAction(float angularVelocity) :
	RotateAction(angularVelocity)
{

}

DefaultRotateAction::~DefaultRotateAction()
{


}

void DefaultRotateAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();

	physicsComponent->applyRotation(m_angularVelocity);

}
