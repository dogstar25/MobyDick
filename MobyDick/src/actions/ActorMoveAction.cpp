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
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>();
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>();

	physicsComponent->applyMovement(vitalityComponent->speed(), m_direction, m_strafe);


	if (animationComponent)
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
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}
	}

}