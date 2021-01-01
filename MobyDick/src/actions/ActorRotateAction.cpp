#include "ActorRotateAction.h"


#include "../GameObject.h"



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
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	physicsComponent->applyRotation(m_angularVelocity);

	if (animationComponent)
	{
		if (m_angularVelocity != 0) {
			animationComponent->animate(ANIMATION_RUN, ANIMATE_ONE_TIME);
		}
	}
}
