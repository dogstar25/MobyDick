#include "DefaultMoveAction.h"


#include "../GameObject.h"


void DefaultMoveAction::perform(GameObject* gameObject, Json::Value actionParms)
{

	assert(actionParms.isMember("direction") && "Direction value is required");
	assert(actionParms.isMember("strafe") && "Strafe value is required");

	//Get action paramters
	auto direction = actionParms["direction"].asInt();
	auto strafe = actionParms["strafe"].asInt();

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	physicsComponent->applyMovement(vitalityComponent->speed(), direction, strafe);

}
