#include "DefaultMoveAction.h"


#include "../GameObject.h"


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
	const auto& physicsComponent = gameObject->physicsComponent.value();
	const auto& vitalityComponent = gameObject->vitalityComponent.value();

	physicsComponent->applyMovement(vitalityComponent->speed(), m_direction, m_strafe);

}
