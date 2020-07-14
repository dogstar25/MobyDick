#include "HeroMoveAction.h"
#include "HeroFireAction.h"
#include <iostream>
#include <memory>

#include "../GameObject.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/VitalityComponent.h"


HeroMoveAction::HeroMoveAction()
{

}

HeroMoveAction::~HeroMoveAction()
{


}

void HeroMoveAction::perform(GameObject* gameObject, int direction, int strafe)
{
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(gameObject->components()[PHYSICS_COMPONENT]);
	auto& animationComponent =
		std::static_pointer_cast<AnimationComponent>(gameObject->components()[ANIMATION_COMPONENT]);
	auto& vitalityComponent =
		std::static_pointer_cast<VitalityComponent>(gameObject->components()[VITALITY_COMPONENT]);

	physicsComponent->applyMovement(vitalityComponent->speed(), direction, strafe);

	if (animationComponent)
	{
		if (direction == 0 && strafe == 0)
		{
			animationComponent->setCurrentAnimationState(ANIMATION_IDLE);
		}
		else
		{
			animationComponent->setCurrentAnimationState(ANIMATION_RUN);
		}
	}

}