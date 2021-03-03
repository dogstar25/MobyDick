#include "DroneMoveAction.h"

#include <iostream>

#include "../Globals.h"
#include "../GameObject.h"


DroneMoveAction::DroneMoveAction() :
	MoveAction(b2Vec2{})
{

}

DroneMoveAction::DroneMoveAction(b2Vec2 trajectory) :
	MoveAction(trajectory)
{

}

DroneMoveAction::~DroneMoveAction()
{


}

void DroneMoveAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	physicsComponent->applyMovement(vitalityComponent->speed(), m_trajectory);


	if (animationComponent)
	{

		if (m_trajectory.Length() != 0)
		{
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}

	}
}
