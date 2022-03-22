#include "ActorRotateAction.h"


#include "../GameObject.h"


void ActorRotateAction::perform(GameObject* gameObject, float angularVelocity)
{
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
