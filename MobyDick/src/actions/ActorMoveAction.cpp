#include "ActorMoveAction.h"

#include <iostream>

#include "../Util.h"
#include "../BaseConstants.h"
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
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	physicsComponent->applyMovement(vitalityComponent->speed(), m_direction, m_strafe);


	if (animationComponent)
	{

		if (m_direction != 0 || m_strafe != 0)
		{
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}

	}

}