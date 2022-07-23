#include "DroneBrainComponent.h"

#include <math.h>
#include <random>
#include <assert.h>
#include <format>

#include "DebugPanel.h"
#include "GameObjectManager.h"


#include "components/VitalityComponent.h"


DroneBrainComponent::DroneBrainComponent(Json::Value definitionJSON)
	: BrainComponent(definitionJSON)
{

	m_engageStateTimer = Timer(5);
	m_eyeFireDelayTimer = Timer(.5);




}

void DroneBrainComponent::postInit()
{
	//Call base brain postinit
	BrainComponent::postInit();

	//Get all WayPoints
	for (const auto& gameObject : parent()->parentScene()->gameObjects()[GameLayer::ABSTRACT]) {

		if (gameObject->hasTrait(TraitTag::navigation)) {

			const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
			if (navComponent->type() == NavigationObjectType::WAYPOINT) {
				m_wayPoints.push_back(gameObject.get());
			}

		}
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

	Json::Value definitionJSON;

	//set back to normal speed
	//ToDo:Need to store original speed instead of going back to json
	//definitionJSON = GameObjectManager::instance().getDefinition(parent()->id())->definitionJSON();
	//Json::Value vitalityComponentJSON = definitionJSON["vitalityComponent"];
	//auto speed = vitalityComponentJSON["speed"].asFloat();
	const auto& vitals = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	//vitals->setSpeed(speed);


	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	// Get the closest waypoint to your current position
	if (m_targetDestination.has_value() == false) {
		m_targetDestination = getClosestNavPoint(transform->getCenterPosition(), NavigationObjectType::WAYPOINT);
	}

	//Have we reached the current target destination.
	//If so, then get the next waypoint destination
	if (util::calculateDistance(parent()->getCenterPosition(), 
		m_targetDestination.value()->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {

		m_targetDestination = getNextPatrolDestination();

		//Clear out the visited itermin nav points now that we've onto
		//a new fresh target destination
		m_tempVisitedNavPoints.clear();

	}


	//Execute the base navigation logic
	navigate();
	//bool atClosestAsPossibleToTarget = navigate();
	//if (atClosestAsPossibleToTarget) {
	//	m_tempVisitedNavPoints.clear();
	//	m_targetDestination = getNextPatrolDestination();
	//}

	//Catch all stuck and need to pick a new destination
	if (_isStuck()) {
		m_tempVisitedNavPoints.clear();
		m_interimDestination.reset();
		m_targetDestination = getNextPatrolDestination();
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

	//std::cout << "Engaging player \n";

	//clear out patrol temp vistied
	m_tempVisitedNavPoints.clear();

	//Make sure interimDestination is populated if off of the track - initialization
	if (m_interimDestination.has_value() == false) {
		m_interimDestination = getClosestNavPoint(parent()->getCenterPosition(), NavigationObjectType::UNSPECIFIED);
	}

	//Set destination to last seen targets location
	m_targetDestination = getClosestNavPoint(m_targetLocation, NavigationObjectType::UNSPECIFIED);

	std::optional<GameObject*> nextInterimDestination = getNextinterimDestination();
	if (nextInterimDestination.has_value()) {
		m_interimDestination = nextInterimDestination.value();
	}

	//DebugPanel::instance().addItem("TargetDest", m_targetDestination.value()->name());
	//DebugPanel::instance().addItem("InterimDest", m_interimDestination.value()->name());

	//Point eye/weapon at target location
	const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	const auto& eyeGameObject = eye.value().gameObject;
	b2Vec2 targetLoc = { m_targetLocation.x ,m_targetLocation.y };
	b2Vec2 rotationCenter{ parent()->getCenterPosition().x, parent()->getCenterPosition().y };
	_rotateTowards(targetLoc, rotationCenter, eyeGameObject.get());

	//Fire eye/weapon
	if (m_eyeFireDelayTimer.hasMetTargetDuration()) {
		auto actionComponent = eyeGameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

		const auto& action = actionComponent->getAction(ACTION_USAGE);
		//action->perform(eyeGameObject.get());
	}

	//Navigate towards target location, unless you are already there
	_navigateEngage();


}

void DroneBrainComponent::_navigateEngage()
{

	//If we have reached the interim destination then find the next possible interim destination that gets us
	//closest to the target dstination
	//Its possible that the interim destination that we find IS the actual target destination we're trying to get to
	//std::optional<std::shared_ptr<GameObject>> nextInterimDestination{ m_interimDestination };
	bool alreadyAtClosestInterimToTargetDestination = false;

	//Has the drone reached the interim destination
	if (util::calculateDistance(parent()->getCenterPosition(), m_interimDestination.value()->getCenterPosition())
		< DESTINATION_DISTANCE_TOLERANCE) {

			alreadyAtClosestInterimToTargetDestination = true;

	}

	//Execute the move actions to get us closer to the interim destination point
	if (alreadyAtClosestInterimToTargetDestination) {
		stopMovement();
	}
	else {
		executeMove();
	}

}


GameObject* DroneBrainComponent::getNextPatrolDestination()
{

	const auto& currentTarget = m_targetDestination.value();
	const auto& currentNavComponent = currentTarget->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

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
	return newDestination;

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
	if (difference < 0.05) {

		action->perform(gameObject, (float)0.);

	}
	else {
		
		action->perform(gameObject, rotationVelocity);

	}

}



int DroneBrainComponent::_determineState()
{
	int state{m_currentState};

	//Do we see the enemy
	std::optional<SDL_FPoint> playerLocation = _detectPlayer();
	if (playerLocation.has_value() ) {

		m_targetLocation = playerLocation.value();
		m_engageStateTimer.reset();
		state = BrainState::ENGAGE;

	}
	else {

		if (m_engageStateTimer.hasMetTargetDuration()) {

			state = BrainState::PATROL;
		}



	}
	
	return state;

}

std::optional<SDL_FPoint> DroneBrainComponent::_detectPlayer()
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

bool DroneBrainComponent::_isStuck()
{
	bool isStuck = {};

	if (m_previousLocation.has_value() == false) {
		m_previousLocation = {-50,-50};
	}

	//Check if we've moved from last iteration
	if (util::calculateDistance(parent()->getCenterPosition(), m_previousLocation.value()) < HASMOVED_DISTANCE_TOLERANCE) {

		if (m_patrolStuckTimer.isSet()) {

			if (m_patrolStuckTimer.hasMetTargetDuration()) {
				isStuck = true;
			}
			else {
				isStuck =  false;
			}
		}
		else {
			m_patrolStuckTimer = { 3 };
			isStuck =  false;
		}

	}
	else {
		m_patrolStuckTimer = { 0 };
	}

	m_previousLocation = parent()->getCenterPosition();

	return isStuck;
}


