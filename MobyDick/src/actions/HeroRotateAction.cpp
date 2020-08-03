#include "HeroRotateAction.h"

#include <iostream>

#include "../GameObject.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"



HeroRotateAction::HeroRotateAction()
{

}

HeroRotateAction::~HeroRotateAction()
{


}

void HeroRotateAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>();

	physicsComponent->applyRotation(m_angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
