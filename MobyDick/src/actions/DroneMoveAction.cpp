#include "DroneMoveAction.h"


#include "../GameObject.h"


void DroneMoveAction::perform(GameObject* gameObject, Json::Value actionParms)
{

	assert(actionParms.isMember("trajectoryX") && "TrajectoryX value is required");
	assert(actionParms.isMember("trajectoryY") && "TrajectoryY value is required");

	//Get action paramters
	auto trajectoryX = actionParms["trajectoryX"].asFloat();
	auto trajectoryY = actionParms["trajectoryY"].asFloat();

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	b2Vec2 trajectory{ trajectoryX , trajectoryY };
	physicsComponent->applyMovement(vitalityComponent->speed(), trajectory);

	if (animationComponent)
	{

		if (trajectory.Length() != 0)
		{
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}

	}
}
