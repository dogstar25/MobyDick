#include "ActorMoveAction.h"


#include "../GameObject.h"


void ActorMoveAction::perform(GameObject* gameObject, int direction, int strafe)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	physicsComponent->applyMovement(vitalityComponent->speed(), direction, strafe);


	if (animationComponent)
	{

		if (direction != 0 || strafe != 0)
		{
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}

	}

}