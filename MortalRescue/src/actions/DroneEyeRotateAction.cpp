#include "DroneEyeRotateAction.h"


#include "GameObject.h"



DroneEyeRotateAction::DroneEyeRotateAction() :
	RotateAction(0)
{

}

DroneEyeRotateAction::DroneEyeRotateAction(float angularVelocity) :
	RotateAction(angularVelocity)
{

}

DroneEyeRotateAction::~DroneEyeRotateAction()
{


}

void DroneEyeRotateAction::perform(GameObject* gameObject)
{
	const auto& physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	//
	if (m_angularVelocity == 0) {
		physicsComponent->setLinearVelocity({ 0, 0 });
	}
	else {
		physicsComponent->applyTorque(m_angularVelocity);
	}
	

}
