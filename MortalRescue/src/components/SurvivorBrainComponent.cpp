#include "SurvivorBrainComponent.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"

#include "../GameConstants.h"



SurvivorBrainComponent::SurvivorBrainComponent(Json::Value definitionJSON)
	: BrainComponent(definitionJSON)
{

	m_currentState = BrainState::IDLE;

}

void SurvivorBrainComponent::update()
{

	BrainComponent::update();

	//Determine state
	m_currentState = _determineState();

	switch (m_currentState) {

	case BrainState::FOLLOW:
		_doFollow();
		break;
	default:
		_doIdle();
		break;
	}

}

void SurvivorBrainComponent::followMe(GameObject* gameObjectToFollow) {

	m_gameObjectToFollow = gameObjectToFollow;
	m_currentState = BrainState::FOLLOW;


}



int SurvivorBrainComponent::_determineState()
{

	int state{ m_currentState };

	////Can we see the enemy/player?
	//std::optional<SDL_FPoint> playerLocation = _detectPlayer();
	//if (playerLocation.has_value()) {

	//	if (m_currentState == BrainState::IDLE) {
	//		state = BrainState::DEPLOY;
	//	}
	//	else if (m_currentState == BrainState::DEPLOY) {

	//		const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	//		//If animation state is ANIMATION_ACTIVE then it has finished the deploy animation
	//		if (animationComponent->currentAnimationState() == ANIMATION_ACTIVE) {
	//			state = BrainState::ENGAGE;
	//		}
	//	}
	//	else if (m_currentState == BrainState::UNDEPLOY) {

	//		const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	//		state = BrainState::DEPLOY;

	//	}
	//	else if (m_currentState == BrainState::ENGAGE) {

	//		//stay engage?

	//	}

	//}
	//else {
	//	if (m_currentState == BrainState::DEPLOY || m_currentState == BrainState::ENGAGE) {
	//		state = BrainState::UNDEPLOY;
	//	}
	//	else if (m_currentState == BrainState::UNDEPLOY) {

	//		//If the undeploy animation is done, then set the brain to IDLE
	//		const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	//		if (animationComponent->currentAnimationState() == ANIMATION_IDLE) {
	//			state = BrainState::IDLE;
	//		}


	//	}
	//}

	return state;

}

void SurvivorBrainComponent::_doFollow()
{




}



void SurvivorBrainComponent::_doIdle()
{
	//const auto& animationComponent = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	//animationComponent->setCurrentAnimationState(ANIMATION_IDLE);

}



bool SurvivorBrainComponent::_rotateTowards(b2Vec2 targetPoint)
{
	bool onTarget{ false };

	auto physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	auto transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	auto currentAngle = physicsComponent->angle();
	auto rotationCenter = transformComponent->getCenterPosition();

	//calculate the angle
	auto desiredAngle = atan2f(
		(targetPoint.y - rotationCenter.y),
		(targetPoint.x - rotationCenter.x));
	desiredAngle = util::normalizeRadians(desiredAngle);

	auto desiredAngleDegrees = util::radiansToDegrees(desiredAngle);
	auto objectAngleDegrees = util::radiansToDegrees(currentAngle);

	float rotationVelocity{ 0 };

	//Get the objects action and vitality components
	auto actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	auto vitality = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if ((desiredAngle - currentAngle) < 0.0) {
		rotationVelocity = vitality->rotateSpeed() * -1;
	}
	else {
		rotationVelocity = vitality->rotateSpeed();
	}

	auto difference = abs(desiredAngle - currentAngle);

	//Once the angle is very close then set the angle directly
	if (difference < 0.19) {

		const auto& action = actionComponent->getAction(ACTION_ROTATE);
		action->perform(parent(), 0);
		onTarget = true;

		//This next code makes the aim dead on balls accurate - may be too hard
		const auto& physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		physicsComponent->setAngle(desiredAngle);
		physicsComponent->update();

	}
	else {
		const auto& action = actionComponent->getAction(ACTION_ROTATE);
		action->perform(parent(), rotationVelocity);
	}

	return onTarget;
}

std::optional<SDL_FPoint> SurvivorBrainComponent::_detectPlayer()
{

	std::optional<SDL_FPoint> playerPosition{};

	for (auto& seenObject : m_seenObjects) {

		if (seenObject.gameObject->hasTrait(TraitTag::player)) {

			playerPosition = seenObject.gameObject->getCenterPosition();
			break;
		}
	}

	return playerPosition;

}
