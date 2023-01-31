#include "DefaultMoveAction.h"

#include "../GameObject.h"

void DefaultMoveAction::perform(GameObject* gameObject, int direction, int strafe)
{
	float speed{10};
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//Vitality component could be optional for this default move action
	if (gameObject->hasComponent(ComponentTypes::VITALITY_COMPONENT)) {
		const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
		speed = vitalityComponent->speed();
	}

	physicsComponent->applyMovement(speed, direction, strafe);

}
