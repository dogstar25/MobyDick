#include "DroneBrainComponent.h"
#include "../GameConstants.h"
#include "game.h"

#include <math.h>
#include <random>
#include <assert.h>
#include <format>
#include <bitset>

#include "DebugPanel.h"
#include "GameObjectManager.h"


#include "components/VitalityComponent.h"

extern std::unique_ptr<Game> game;


DroneBrainComponent::DroneBrainComponent(Json::Value definitionJSON)
	: BrainComponent(definitionJSON)
{

	m_engageStateTimer = Timer(3);
	m_eyeFireDelayTimer = Timer(.5, true);



}

void DroneBrainComponent::postInit()
{
	//Call base brain postinit
	BrainComponent::postInit();

	//Get all WayPoints
	for (const auto& gameObject : parent()->parentScene()->getGameObjectsByTrait(TraitTag::waypoint)) {

		m_wayPoints.push_back(gameObject);
	}

	//Do an random sort of the waypoints order
	unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(m_wayPoints.begin(), m_wayPoints.end(), std::default_random_engine(seed));

}

void DroneBrainComponent::update()
{
	//Call base update
	BrainComponent::update();

	//Determine state
	m_currentState = _determineState();

	switch (m_currentState) {

		case BrainState::PATROL:
			_doPatrol();
			break;
		case BrainState::ENGAGE:
			_doEngage();
			break;
		default:
			_doPatrol();
			break;
	}
	
}

void DroneBrainComponent::_doPatrol()
{

	NavigationStatus navigationCode{};
	auto navigationComponent = parent()->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

	if (m_focusPoint.has_value() == false) {
		m_focusPoint = _getNextPatrolDestination();
	}

	navigationCode = navigationComponent->navigateTo(m_focusPoint.value().x, m_focusPoint.value().y);

	if (navigationCode == NavigationStatus::DESTINATION_REACHED) {
		m_focusPoint = _getNextPatrolDestination();
		assert(m_focusPoint.has_value() && "No Patrol Destination was set");
	}
	else if (navigationCode == NavigationStatus::NO_PATH_FOUND) {
		m_focusPoint = _getNextPatrolDestination();
		//std::cout << "Drone! No path was found for navigation!" << std::endl;
	}
	else if (navigationCode == NavigationStatus::STUCK) {

		if (m_currentState == BrainState::PATROL) {
			m_focusPoint = _getNextPatrolDestination();
		}
	}

}

void DroneBrainComponent::_doAlert()
{
}

void DroneBrainComponent::_doPursue()
{



}

void DroneBrainComponent::_doEngage()
{
	NavigationStatus navigationCode{};

	auto navigationComponent = parent()->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);


	//Point eye/weapon at target location
	const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	const auto& eyeGameObject = eye.value().gameObject;
	b2Vec2 targetLoc = { m_focusPoint.value().x ,m_focusPoint.value().y };
	b2Vec2 rotationCenter{ parent()->getCenterPosition().x, parent()->getCenterPosition().y };
	_rotateTowards(targetLoc, rotationCenter, eyeGameObject.get());

	//Fire eye/weapon
	if (m_eyeFireDelayTimer.hasMetTargetDuration()) {
		auto actionComponent = eyeGameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

		//Instead, get the weapon component and execute fire - PROPOSAL FOR ACTIONcOMPONENT CHANGES
		//const auto& weaponComponent = eyeGameObject->getComponent<WeaponComponent>(ComponentTypes::WEAPON_COMPONENT);
		//weaponComponent->fire();


		const auto& action = actionComponent->getAction(ACTION_USAGE);
		action->perform(eyeGameObject.get());
	}

	//If we are pretty close to the player then stop but should keep shooting
	// but dont try to navigate
	auto playerDistance = util::calculateDistance(parent()->getCenterPosition(), m_focusPoint.value());
	auto objectSize = parent()->getSize().x;
	if (playerDistance > (objectSize * 2)) {

		//Navigate towards target location, unless you are already there
		navigationCode = navigationComponent->navigateTo(m_focusPoint.value().x, m_focusPoint.value().y);

		//If we cant get to the player location then stop navigating
		if (navigationCode == NavigationStatus::NO_PATH_FOUND) {

			navigationComponent->navigateStop();
		}

	}
	else {

		navigationComponent->navigateStop();
	}


}



SDL_FPoint DroneBrainComponent::_getNextPatrolDestination()
{

	SDL_FPoint newPatrolLocation{};

	//Get the next waypoint in the list
	//If we have reached the end then randomly resort the waypoints and reset to zero
	if (m_currentWaypointIndex == m_wayPoints.size() - 1) {

		m_currentWaypointIndex = 0;
		unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
		std::shuffle(m_wayPoints.begin(), m_wayPoints.end(), std::default_random_engine(seed));

	}
	else {
		m_currentWaypointIndex += 1;
	}

	auto newDestination = m_wayPoints[m_currentWaypointIndex];
	newPatrolLocation = newDestination.lock().get()->getCenterPosition();

	//newPatrolLocation = util::pixelToTileLocation(newPatrolLocation.x, newPatrolLocation.y);

	return newPatrolLocation;

}


void DroneBrainComponent::_rotateTowards(b2Vec2 targetPoint, b2Vec2 rotationCenter, GameObject* gameObject)
{

	auto physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	auto currentAngle = physicsComponent->angle();

	auto desiredAngle = atan2f(
		(targetPoint.y - rotationCenter.y),
		(targetPoint.x - rotationCenter.x));
	desiredAngle = util::normalizeRadians(desiredAngle);

	auto desiredAngleDegrees = util::radiansToDegrees(desiredAngle);
	auto objectAngleDegrees = util::radiansToDegrees(currentAngle);

	float rotationVelocity{ 0 };

	//Get the objects action and vitality components
	auto actionComponent = gameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	auto vitality = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);


	//TODO: save the last ABS of the difference and compare and adjust forward
	//or back based on that to zero-in on target

	if ((desiredAngle - currentAngle) < 0.0) {
		rotationVelocity = vitality->rotateSpeed() * -1;
	}
	else {
		rotationVelocity = vitality->rotateSpeed();
	}

	auto difference = abs(desiredAngle - currentAngle);

	const auto& action = actionComponent->getAction(ACTION_ROTATE);

	//Once the angle is very close then set the angle directly
	if (difference < 0.2) {

		action->perform(gameObject, (float)0.);

	}
	else {
		
		action->perform(gameObject, rotationVelocity);

	}

}



int DroneBrainComponent::_determineState()
{
	int state{ m_currentState };

	//Do we see the enemy
	std::optional<SDL_FPoint> playerLocation = _detectPlayer();
	if (playerLocation.has_value()) {

		m_focusPoint = playerLocation.value();
		m_engageStateTimer.reset();
		state = BrainState::ENGAGE;

	}
	else {

		if (m_currentState == BrainState::ENGAGE) {

			if (m_engageStateTimer.hasMetTargetDuration()) {

				m_focusPoint = std::nullopt;
				state = BrainState::PATROL;

			}
			else {

				//get the payers actual position
				auto player = parent()->parentScene()->getGameObjectsByTrait(TraitTag::player)[0];
				m_focusPoint = player->getCenterPosition();

			}

		}
		else if (m_currentState == BrainState::PATROL) {

			state = BrainState::PATROL;

		}

	}

	return state;
}


std::optional<SDL_FPoint> DroneBrainComponent::_detectPlayer()
{

	std::optional<SDL_FPoint> playerPosition{};

	for (auto& seenObject : m_seenObjects) {

		if (seenObject.gameObject.expired() == false) {

			//Is this the hitbox fixture of the PlayerObject
			ContactDefinition* contactDefinition = reinterpret_cast<ContactDefinition*>(seenObject.fixture->GetUserData().pointer);
			if (contactDefinition->contactTag == ContactTag::PLAYER_HITBOX)
			{

				if (seenObject.gameObject.expired() == false && seenObject.gameObject.lock()->hasTrait(TraitTag::player)
					&& seenObject.gameObject.lock()->isAlive())
				{

					playerPosition = seenObject.gameObject.lock()->getCenterPosition();
					break;
				}
			}
		}
	}

	return playerPosition;

}




