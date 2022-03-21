#include "DefaultRotateAction.h"



#include "../GameObject.h"



void DefaultRotateAction::perform(GameObject* gameObject, Json::Value actionParms)
{

	assert(actionParms.isMember("angularVelocity") && "AngularVelocity value is required");

	//Get action paramters
	float angularVelocity = actionParms["angularVelocity"].asFloat();

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	physicsComponent->applyRotation(angularVelocity);

}
