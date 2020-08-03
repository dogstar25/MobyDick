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

void HeroMoveAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>();
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>();

	physicsComponent->applyMovement(vitalityComponent->speed(), m_direction, m_strafe);

	if (animationComponent)
	{
		if (m_direction == 0 && m_strafe == 0)
		{
			animationComponent->setCurrentAnimationState(ANIMATION_IDLE);
		}
		else
		{
			animationComponent->setCurrentAnimationState(ANIMATION_RUN);
		}
	}

}