#include "ActorRotateAction.h"


#include "../GameObject.h"


void ActorRotateAction::perform(GameObject* gameObject, Json::Value actionParms)
{

	assert(actionParms.isMember("angularVelocity") && "AngularVelocity value is required");

	//Get action paramters
	float angularVelocity = actionParms["angularVelocity"].asFloat();

	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	physicsComponent->applyRotation(angularVelocity);

	if (animationComponent)
	{
		if (angularVelocity != 0) {
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}
	}
}
