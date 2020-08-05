#include "ActorRotateAction.h"

#include <iostream>

#include "../GameObject.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"



ActorRotateAction::ActorRotateAction() :
	RotateAction(0)
{

}

ActorRotateAction::ActorRotateAction(float angularVelocity) :
	RotateAction(angularVelocity)
{

}

ActorRotateAction::~ActorRotateAction()
{


}

void ActorRotateAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>();
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>();

	physicsComponent->applyRotation(m_angularVelocity);

	if (animationComponent)
	{
		animationComponent->setCurrentAnimationState(ANIMATION_RUN);
	}
}
