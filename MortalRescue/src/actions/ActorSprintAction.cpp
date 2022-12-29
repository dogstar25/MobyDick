#include "ActorSprintAction.h"


#include "GameObject.h"


void ActorSprintAction::perform(GameObject* gameObject, int direction, int strafe)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	physicsComponent->applyMovement(vitalityComponent->speed() * 1.5f, direction, strafe);


	if (animationComponent && (direction != 0 || strafe != 0))
	{

		animationComponent->animate(ANIMATION_SPRINT, ANIMATE_ONE_TIME);

	}


}