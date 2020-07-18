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

void HeroRotateAction::perform(GameObject* gameObject, float angularVelocity )
{
	auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	auto& animationComponent = gameObject->getComponent<AnimationComponent>();

	physicsComponent->applyRotation(angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
