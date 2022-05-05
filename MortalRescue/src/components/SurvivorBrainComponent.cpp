#include "SurvivorBrainComponent.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "game.h"

#include "../GameConstants.h"

extern std::unique_ptr<Game> game;


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

void SurvivorBrainComponent::stay() {

	m_gameObjectToFollow = nullptr;
	m_currentState = BrainState::IDLE;

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

	b2Vec2 trajectory{};

	//If we are not closeenough to the object we're following then move to within tolerance
	if (util::calculateDistance(parent()->getCenterPosition(), m_gameObjectToFollow->getCenterPosition())
		> SURVIVOR_FOLLOW_TOLERANCE) {

		trajectory.x = m_gameObjectToFollow->getCenterPosition().x - parent()->getCenterPosition().x;
		trajectory.y = m_gameObjectToFollow->getCenterPosition().y - parent()->getCenterPosition().y;

		trajectory.Normalize();

		const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), trajectory);

	}

	//Stay behind the object you are following
	const auto& gameObjectToFollowPhysicsComponent = m_gameObjectToFollow->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& gameObjectToFollowTransformComponent = m_gameObjectToFollow->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	ImGui::SetNextWindowPos(ImVec2{ 350, 0 });
	ImGui::Begin("Survivor Stay Behind");

	float gameObjectAngleDegrees = util::radiansToDegrees(gameObjectToFollowPhysicsComponent->angle());
	float orientationAngle = atan2(
		parent()->getCenterPosition().y - m_gameObjectToFollow->getCenterPosition().y,
		parent()->getCenterPosition().x - m_gameObjectToFollow->getCenterPosition().x
	);

	float orientationAngleDegrees = util::radiansToDegrees(orientationAngle);

	float difference = gameObjectAngleDegrees - orientationAngleDegrees;
	//Shooting in the dark here
	if (!((difference > 90 && difference < 270) ||
		(difference > -180 && difference < -90))) {

		ImGui::Text("GET BEHIND!");

		//Rotate to the left or the right based on the position
		int sinDirection{};
		int cosDirection{};
		if (difference < 90) {
			sinDirection = 1;
			cosDirection = -1;
		}
		else {
			sinDirection = -1;
			cosDirection = 1;
		}

		//Rotate myself around the object im following until im behind
		trajectory.x = (sin(orientationAngle) * 200 * sinDirection) + (m_gameObjectToFollow->getCenterPosition().x - parent()->getCenterPosition().x);
		trajectory.y = (cos(orientationAngle) * 200 * cosDirection) + (m_gameObjectToFollow->getCenterPosition().y - parent()->getCenterPosition().y);
		trajectory.Normalize();

		const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), trajectory);

	}

	SDL_FPoint destination{};
	destination.x = m_gameObjectToFollow->getCenterPosition().x + (64 * cos(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(135)));
	destination.y = m_gameObjectToFollow->getCenterPosition().y + (64 * sin(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(135)));

	SDL_FPoint destination2{};
	destination2.x = m_gameObjectToFollow->getCenterPosition().x + (64 * cos(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(180)));
	destination2.y = m_gameObjectToFollow->getCenterPosition().y + (64 * sin(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(180)));

	SDL_FPoint destination3{};
	destination3.x = m_gameObjectToFollow->getCenterPosition().x + (64 * cos(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(225)));
	destination3.y = m_gameObjectToFollow->getCenterPosition().y + (64 * sin(gameObjectToFollowPhysicsComponent->angle() + util::degreesToRadians(225)));


	glm::vec2 start{ m_gameObjectToFollow->getCenterPosition().x , m_gameObjectToFollow->getCenterPosition().y };
	glm::vec2 stop{ destination.x , destination.y };
	game->renderer()->addLine(start, stop, { 255,255,255,255 });
	start = { m_gameObjectToFollow->getCenterPosition().x , m_gameObjectToFollow->getCenterPosition().y };
	stop = { destination2.x , destination2.y };
	game->renderer()->addLine(start, stop, { 255,255,255,255 });
	start = { m_gameObjectToFollow->getCenterPosition().x , m_gameObjectToFollow->getCenterPosition().y };
	stop = { destination3.x , destination3.y };
	game->renderer()->addLine(start, stop, { 255,255,255,255 });



	ImGui::Value("gameObjectAngleDegrees", gameObjectAngleDegrees);
	ImGui::Value("Orientation Angle", orientationAngleDegrees);
	ImGui::Value("Difference", difference);

	ImGui::End();


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
