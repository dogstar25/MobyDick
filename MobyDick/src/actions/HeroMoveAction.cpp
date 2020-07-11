#include "HeroMoveAction.h"
#include "HeroFireAction.h"
#include <iostream>

#include "../GameObject.h"


HeroMoveAction::HeroMoveAction()
{

}

HeroMoveAction::~HeroMoveAction()
{


}

void HeroMoveAction::perform(
	std::shared_ptr<PhysicsComponent> physicsComponent, 
	std::shared_ptr<AnimationComponent> animationComponent, 
	float speed, 
	int direction, 
	int strafe)
{

	physicsComponent->applyMovement(speed, direction, strafe);

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