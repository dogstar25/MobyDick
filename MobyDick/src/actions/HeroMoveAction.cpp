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
	auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	auto& animationComponent = gameObject->getComponent<AnimationComponent>();
	auto& vitalityComponent = gameObject->getComponent<VitalityComponent>();

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