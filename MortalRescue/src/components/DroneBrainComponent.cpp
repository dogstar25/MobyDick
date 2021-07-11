#include "DroneBrainComponent.h"

#include "DebugPanel.h"
#include "GameObjectManager.h"
#include <assert.h>

#include "components/VitalityComponent.h"
#include <math.h>
#include <random>


DroneBrainComponent::DroneBrainComponent(Json::Value definitionJSON)
	: BrainComponent(definitionJSON)
{

	m_engageStateTimer = Timer(5);




}

void DroneBrainComponent::postInit()
{

	//Get all NavPoints, including ones that are waypoints
	for (const auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT]) {

		if (gameObject->hasTrait(TraitTag::navigation)) {

			const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

			m_navPoints.push_back(gameObject.get());

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
	

	//Test
	/*const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	const auto& eyeActionComponent = eye->gameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	auto direction = util::generateRandomNumber(1, 100);
	if (direction < 90) {
		eyeActionComponent->performRotateAction(0);
	}
	else {
		eyeActionComponent->performRotateAction(25);
	}*/

	


	//const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);

	//const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	//const auto& eyePhysicsComponent = eye->gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//auto degrees = util::generateRandomNumber(1, 10);
	//auto direction = util::generateRandomNumber(1, 100);
	//if (direction < 50) {
	//	degrees *= -1;
	//}

	//auto force = degrees * direction;

	//eyePhysicsComponent->applyAngleImpulse(force);




}

void DroneBrainComponent::_doPatrol()
{

	Json::Value definitionJSON;

	//set back to normal speed
	//ToDo:Need to store original speed instead of going back to json
	definitionJSON = GameObjectManager::instance().getDefinition(parent()->id())->definitionJSON();
	Json::Value vitalityComponentJSON = definitionJSON["vitalityComponent"];
	auto speed = vitalityComponentJSON["speed"].asFloat();
	const auto& vitals = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
	vitals->setSpeed(speed);


	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	// Get the closest waypoint to your current position
	if (m_targetDestination.has_value() == false) {
		m_targetDestination = getClosestNavPoint(transform->getCenterPosition(), NavigationObjectType::WAYPOINT);
	}

	//Have we reached the current target destination.
	//If so, then get the next waypoint destination
	if (calculateDistance(parent()->getCenterPosition(), 
		m_targetDestination.value()->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {

		m_targetDestination = getNextPatrolDestination();

		//Clear out the visited itermin nav points now that we've onto
		//a new fresh target destination
		m_tempVisitedNavPoints.clear();

	}

	navigatePatrol();

}


void DroneBrainComponent::_doAlert()
{
}

void DroneBrainComponent::_doPursue()
{



}

void DroneBrainComponent::_doEngage()
{

	std::cout << "Engaging player \n";

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

	DebugPanel::instance().addItem("TargetDest", m_targetDestination.value()->name());
	DebugPanel::instance().addItem("InterimDest", m_interimDestination.value()->name());



	//ToDo: point weapon at target location
	const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	const auto& eyeGameObject = eye.value().gameObject;
	b2Vec2 targetLoc = { m_targetLocation.x ,m_targetLocation.y };
	b2Vec2 rotationCenter{ parent()->getCenterPosition().x, parent()->getCenterPosition().y };
	_rotateTowards(targetLoc, rotationCenter, eyeGameObject.get());

	//ToDo: fire weapon at target location

	//Navigate towards target location, unless you are already there
	navigateEngage();


}

GameObject* DroneBrainComponent::getClosestNavPoint(SDL_FPoint targetPosition, int navType)
{

	//For each navigation item, calculate the distance from the given position
	float shortest{};
	GameObject* closestWayPoint{};

	/*
	* Find the closest nav point that is within line of sight
	*/
	for (const auto& seenObject : m_seenObjects) {

		if (seenObject.gameObject->hasTrait(TraitTag::navigation)) {

			const auto& navComponent = seenObject.gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

			if (navComponent->type() == navType || navType == NavigationObjectType::UNSPECIFIED) {
				const auto& transform =
					seenObject.gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
				auto navPosition = transform->getCenterPosition();

				auto distance = std::powf((targetPosition.x - navPosition.x), 2) + std::powf((targetPosition.y - navPosition.y), 2);
				distance = std::sqrtf(distance);

				if (shortest == 0. || shortest > distance) {
					shortest = distance;
					closestWayPoint = seenObject.gameObject;
				}
			}

		}
			


	}

	if (!closestWayPoint) {



		for (const auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT]) {

			if (gameObject->hasTrait(TraitTag::navigation)) {

				const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
				if (navComponent->type() == navType || navType == NavigationObjectType::UNSPECIFIED) {

					const auto& transform = gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
					auto navPosition = transform->getCenterPosition();

					//distance calculation
					auto distance = std::powf((targetPosition.x - navPosition.x), 2) + std::powf((targetPosition.y - navPosition.y), 2);
					distance = std::sqrtf(distance);

					if (shortest == 0. || shortest > distance) {
						shortest = distance;
						closestWayPoint = gameObject.get();
					}
				}
			}

		}
	}


	assert(closestWayPoint != nullptr && "DroneBrain: No navpoint was found!");

	return closestWayPoint;
}

void DroneBrainComponent::navigatePatrol()
{

	//If we do not have an interim destination then we are off the nav path so get to the nearest one
	if (m_interimDestination.has_value() == false) {
		m_interimDestination = getClosestNavPoint(parent()->getCenterPosition(), NavigationObjectType::UNSPECIFIED);
	}

	//If we have reached the interim destination then find the next possible interim destination that gets us
	//closest to the target dstination
	//Its possible that the interim destination that we find IS the actual target destination we're trying to get to
	//std::optional<std::shared_ptr<GameObject>> nextInterimDestination{ m_interimDestination };
	bool alreadyAtClosestInterimToTargetDestination = false;

	//Has the drone reached the interim destination
	if (calculateDistance( parent()->getCenterPosition(), m_interimDestination.value()->getCenterPosition())
		< DESTINATION_DISTANCE_TOLERANCE) {

		//Now that we have hit this interim nav point, add it to a list of visted nav points
		//so that we can avoid these while trying to navigate to the ultimate target destination
		//while in patrol mode
		m_tempVisitedNavPoints.push_back(m_interimDestination.value());

		//Looks for next interim destination. null_Opt is returned if we alerady are at the
		//closest nav point to the targetDestination
		std::optional<GameObject*> nextInterimDestination = getNextinterimDestination();
		if (nextInterimDestination.has_value()) {
			m_interimDestination = nextInterimDestination.value();
		}
		else {
			alreadyAtClosestInterimToTargetDestination = true;
		}

	}

	//Execute the move actions to get us closer to the interim destination point
	if (alreadyAtClosestInterimToTargetDestination) {
		_stopMovement();
	}
	else {
		_executeMove();
	}
	


}

void DroneBrainComponent::navigateEngage()
{

	//If we have reached the interim destination then find the next possible interim destination that gets us
	//closest to the target dstination
	//Its possible that the interim destination that we find IS the actual target destination we're trying to get to
	//std::optional<std::shared_ptr<GameObject>> nextInterimDestination{ m_interimDestination };
	bool alreadyAtClosestInterimToTargetDestination = false;

	//Has the drone reached the interim destination
	if (calculateDistance(parent()->getCenterPosition(), m_interimDestination.value()->getCenterPosition())
		< DESTINATION_DISTANCE_TOLERANCE) {

			alreadyAtClosestInterimToTargetDestination = true;

	}

	//Execute the move actions to get us closer to the interim destination point
	if (alreadyAtClosestInterimToTargetDestination) {
		_stopMovement();
	}
	else {
		_executeMove();
	}



}

void DroneBrainComponent::_executeMove()
{
	GameObject* interim = m_interimDestination.value();
	const auto& interimNavComponent = interim->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	b2Vec2 trajectory{};
	trajectory.x = interimNavComponent->getCenterPosition().x - parent()->getCenterPosition().x;
	trajectory.y = interimNavComponent->getCenterPosition().y - parent()->getCenterPosition().y;

	trajectory.Normalize();

	auto action = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	action->performMoveAction(trajectory);

	//Possibly adjust movement with small impulse moves to avoid brushing obstacles
	//_applyAvoidanceMovement();

	//Set the angle to point towards the next nav point using the trajectory we calculated above
	//_rotateTowards(trajectory, parent());


}

void DroneBrainComponent::_stopMovement()
{
	b2Vec2 trajectory{0,0};

	auto action = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	action->performMoveAction(trajectory);

}



float DroneBrainComponent::calculateDistance(SDL_FPoint location1, SDL_FPoint location2)
{

	auto distance = std::powf((location1.x - location2.x), 2) + std::powf((location1.y - location2.y), 2);
	distance = std::sqrtf(distance);

	return distance;
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

/*
Find the accessible iterim point that is the closest to the current target 
*/
std::optional<GameObject*> DroneBrainComponent::getNextinterimDestination()
{
	GameObject* foundNavGameObject;

	const auto& target = m_targetDestination.value();
	const auto& targetTransformComponent = target->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	const auto& currentInterim = m_interimDestination.value();
	const auto& interimNavComponent = currentInterim->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
	const auto& interimTransformComponent = currentInterim->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);


	std::optional<float> shortestDistance{};
	std::optional<GameObject*>shortestDistanceObject{};

	//Are we already at the closest nav point possible
	if (calculateDistance(interimTransformComponent->getCenterPosition(),
		targetTransformComponent->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {
		shortestDistanceObject = std::nullopt;
	}
	else {

		for (auto& navPoint : interimNavComponent->accessibleNavObjects()) {

			if (existsInAlreadyVistedNavList(navPoint.get()) == false) {
				const auto& navPointTemp = navPoint;
				const auto& navPointTransformComponent = navPointTemp->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

				//Calulate the distance from this accessible nav point and the target waypoint
				float distance = calculateDistance(navPointTransformComponent->getCenterPosition(), targetTransformComponent->getCenterPosition());

				if (shortestDistance.has_value() == false || shortestDistance.value() > distance) {

					shortestDistance = distance;
					shortestDistanceObject = navPointTemp.get();

				}
			}
		}

	}
	
	return shortestDistanceObject;

}

bool DroneBrainComponent::existsInAlreadyVistedNavList(GameObject* navPoint)
{
	bool found{ false };

	for (const auto vistedPoint : m_tempVisitedNavPoints) {

		if (navPoint == vistedPoint) {
			found = true;
		}
	}

	return found;
}

void DroneBrainComponent::_rotateTowards(b2Vec2 targetPoint, b2Vec2 rotationCenter, GameObject* gameObject)
{

	//auto physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	auto physicsComponent = gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	auto currentAngle = physicsComponent->angle();

	auto dronePosition = std::format("{:.1f},  {:.1f}", 
		rotationCenter.x,
		rotationCenter.y);

	auto eyePosition = std::format("{:.1f},   {:.1f}",
		rotationCenter.x,
		rotationCenter.y);

	DebugPanel::instance().addItem("Drone Position", dronePosition);
	DebugPanel::instance().addItem("Eye Position", eyePosition);

	auto desiredAngle = atan2f(
		targetPoint.x - rotationCenter.x,
		-(targetPoint.y - rotationCenter.y));
	desiredAngle = util::normalizeRadians(desiredAngle);

	auto desiredAngleDegrees = util::radiansToDegrees(desiredAngle);
	auto objectAngleDegrees = util::radiansToDegrees(currentAngle);

	DebugPanel::instance().addItem("Object Angle", objectAngleDegrees, 2);
	DebugPanel::instance().addItem("Desired Angle", desiredAngleDegrees, 2);

	float rotationVelocity{ 0 };

	//Get the objects action and vitality components
	auto action = gameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	auto vitality = gameObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if ((desiredAngle - currentAngle) < 0.0) {
		rotationVelocity = vitality->rotateSpeed() * -1;
	}
	else {
		rotationVelocity = vitality->rotateSpeed();
	}

	auto difference = abs(desiredAngle - currentAngle);
	DebugPanel::instance().addItem("Angle Diff in Radians", desiredAngle - currentAngle, 2);

	//Once the angle is very close then set the angle directly
	if (difference < 0.05) {
		action->performRotateAction(0);
	}
	else {
		
		action->performRotateAction(rotationVelocity);
	}
	


}

//void DroneBrainComponent::_applyAvoidanceMovement()
//{
//
//	const auto& physics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
//
//	//Check all sensor detected objects and see if any are close enough to prompt a movement adjustment
//	DebugPanel::instance().addItem("Distance Hit: ", "");
//	for (const auto& intersectionItem : m_seenObjects) {
//
//
//		if (intersectionItem.fraction * 25 < 5) {
//
//			DebugPanel::instance().addItem("Seen Object Distance: ", intersectionItem.fraction * 25, 5);
//			DebugPanel::instance().addItem("Distance Hit: ", "HIT");
//
//			DebugPanel::instance().addItem("PULSE: ", "");
//			if (intersectionItem.gameObject->hasTrait(TraitTag::barrier)) {
//				DebugPanel::instance().addItem("PULSE: ", "PULSE");
//				DebugPanel::instance().addItem("TrajectoryX: ", intersectionItem.normal.x, 1);
//				DebugPanel::instance().addItem("TrajectoryY: ", intersectionItem.normal.y, 1);
//				auto trajectory = intersectionItem.normal;
//				physics->applyImpulse(300000, trajectory);
//
//
//
//
//				const auto& transform = intersectionItem.gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//				b2Vec2 centerPosition = { transform->getCenterPosition().x , transform->getCenterPosition().y };
//
//				b2Vec2 begin = centerPosition;
//				b2Vec2 end = trajectory;
//
//				auto lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
//				auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//				lineTransform->setLine(begin, end);
//
//
//
//
//			}
//
//		}
//
//	}
//
//}


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


