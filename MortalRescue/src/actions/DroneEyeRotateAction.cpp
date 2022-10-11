#include "DroneEyeRotateAction.h"


#include "GameObject.h"



void DroneEyeRotateAction::perform(GameObject* gameObject, float angularVelocity)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	if (angularVelocity == 0) {
		physicsComponent->setLinearVelocity({ 0, 0 });
	}
	else {
		physicsComponent->applyRotation(angularVelocity);
		//physicsComponent->applyTorque(angularVelocity);
	}
	

}
