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
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(gameObject->components()[PHYSICS_COMPONENT]);
	auto& animationComponent =
		std::static_pointer_cast<AnimationComponent>(gameObject->components()[ANIMATION_COMPONENT]);

	physicsComponent->applyRotation(angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
