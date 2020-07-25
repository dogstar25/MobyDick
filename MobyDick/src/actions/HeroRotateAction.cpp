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
	auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	auto& animationComponent = gameObject->getComponent<AnimationComponent>();

	physicsComponent->applyRotation(m_angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
