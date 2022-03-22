#include "DefaultRotateAction.h"

#include "../GameObject.h"


void DefaultRotateAction::perform(GameObject* gameObject, float angularVelocity)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	physicsComponent->applyRotation(angularVelocity);

}
