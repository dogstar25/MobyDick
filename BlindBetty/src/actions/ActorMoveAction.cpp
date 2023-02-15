#include "ActorMoveAction.h"


#include "GameObject.h"


void ActorMoveAction::perform(GameObject* gameObject, int direction, int strafe)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if (strafe != 0 || direction == -1) {
		//physicsComponent->applyImpulse(vitalityComponent->speed(), direction, strafe);
		physicsComponent->applyMovement(vitalityComponent->speed(), direction, strafe);
	}


	if (animationComponent)
	{

		if (direction != 0 || strafe != 0)
		{
			if (strafe < 0) {
				animationComponent->animate(AnimationState::WALK_LEFT, ANIMATE_ONE_TIME);
				animationComponent->setDefaultAnimationState(AnimationState::IDLE_LEFT);
			}
			else {
				animationComponent->animate(AnimationState::WALK_RIGHT, ANIMATE_ONE_TIME);
				animationComponent->setDefaultAnimationState(AnimationState::IDLE_RIGHT);
			}
		}
		else {

			animationComponent->setToDefaultAnimation();
		}

	}

}