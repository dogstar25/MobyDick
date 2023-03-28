#include "ActorStairsMoveAction.h"

#include "../GameConstants.h"
#include "GameObject.h"


void ActorStairsMoveAction::perform(GameObject* gameObject, int direction)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if (direction == -1) {
		physicsComponent->applyMovement(vitalityComponent->speed(), direction, 0);
	}
	else {
		physicsComponent->applyMovement(vitalityComponent->speed(), direction, 0);
	}

	if (animationComponent)
	{

		if (direction == -1)
		{
			animationComponent->animate(AnimationState::WALK_DOWN, ANIMATE_ONE_TIME);
			animationComponent->setDefaultAnimationState(AnimationState::WALK_DOWN);
		}
		else if (direction == 1){
			animationComponent->animate(AnimationState::WALK_DOWN, ANIMATE_ONE_TIME);
			animationComponent->setDefaultAnimationState(AnimationState::WALK_DOWN);
		}
		else {

			if (animationComponent->currentAnimationState() == AnimationState::WALK_DOWN) {
				animationComponent->animate(AnimationState::WALK_DOWN_IDLE, ANIMATE_ONE_TIME);
			}
			else {
				animationComponent->animate(AnimationState::WALK_DOWN_IDLE, ANIMATE_ONE_TIME);
			}

		}

	}

}