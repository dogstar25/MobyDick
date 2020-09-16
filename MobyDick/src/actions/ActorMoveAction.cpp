#include "ActorMoveAction.h"

#include "../GameObject.h"


ActorMoveAction::ActorMoveAction() :
	MoveAction(0, 0)
{

}

ActorMoveAction::ActorMoveAction(int direction, int strafe) : 
	MoveAction(direction, strafe)
{

}

ActorMoveAction::~ActorMoveAction()
{


}

void ActorMoveAction::perform(GameObject* gameObject)
{
	if (gameObject->physicsComponent) {
		gameObject->physicsComponent.value()->applyMovement(gameObject->vitalityComponent.value()->speed(), m_direction, m_strafe);
	}


	if (gameObject->animationComponent)
	{

		//std::cout << "\033[1;32mMoving\033[0m" << "\n";

		if (m_direction != 0 || m_strafe != 0)
		//{
		//	//animationComponent->setCurrentAnimationState(ANIMATION_IDLE);
		//	animationComponent->animate(ANIMATION_IDLE, ANIMATE_ONE_TIME);
		//}
		//else
		{
			//animationComponent->setCurrentAnimationState(ANIMATION_RUN);
			gameObject->animationComponent.value()->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}
	}

}