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

void HeroMoveAction::perform(GameObject* gameObject, int direction, int strafe)
{

	gameObject->physicsComponent()->applyMovement(gameObject->vitalityComponent()->speed(), direction, strafe);

	if (gameObject->animationComponent())
	{
		if (direction == 0 && strafe == 0)
		{
			gameObject->animationComponent()->setCurrentAnimationState(ANIMATION_IDLE);
		}
		else
		{
			gameObject->animationComponent()->setCurrentAnimationState(ANIMATION_RUN);
		}
	}

}