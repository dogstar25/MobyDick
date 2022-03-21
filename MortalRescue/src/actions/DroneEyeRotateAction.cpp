#include "DroneEyeRotateAction.h"


#include "GameObject.h"



void DroneEyeRotateAction::perform(GameObject* gameObject, Json::Value actionParms)
{


	assert(actionParms.isMember("angularVelocity") && "AngularVelocity value is required");

	//Get action paramters
	auto angularVelocity = actionParms["angularVelocity"].asFloat();

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	if (angularVelocity == 0) {
		physicsComponent->setLinearVelocity({ 0, 0 });
	}
	else {
		physicsComponent->applyTorque(angularVelocity);
	}
	

}
