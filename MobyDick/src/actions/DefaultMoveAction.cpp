#include "DefaultMoveAction.h"

#include <iostream>
#include <memory>

#include "../GameObject.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/VitalityComponent.h"


DefaultMoveAction::DefaultMoveAction() :
	MoveAction(0, 0)
{

}

DefaultMoveAction::DefaultMoveAction(int direction, int strafe) :
	MoveAction(direction, strafe)
{

}

DefaultMoveAction::~DefaultMoveAction()
{

}

void DefaultMoveAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>();

	physicsComponent->applyMovement(vitalityComponent->speed(), m_direction, m_strafe);

}
