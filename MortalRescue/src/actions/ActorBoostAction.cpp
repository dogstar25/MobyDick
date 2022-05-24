#include "ActorBoostAction.h"


#include "GameObject.h"


void ActorBoostAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	const auto& vitalityComponent = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	//Disable the linear drag so that we can "fly"
	//physicsComponent->setLinearDamping(0.0);

	auto currentActorAngle = physicsComponent->angle();
	b2Vec2 trajectory{ cos(currentActorAngle) , sin(currentActorAngle)};
	trajectory.Normalize();

	physicsComponent->applyMovement(20, trajectory);

	animationComponent->animate(ANIMATION_IDLE, ANIMATE_CONTINUOUS);

}