#include "DroneBrainComponent.h"

#include "DebugPanel.h"

#include <math.h>
#include <random>



void DroneBrainComponent::postInit()
{

	//Get all NavPoints, including ones that are waypoints
	for (const auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT]) {

		if (gameObject->idTag() == IdTag::NAVIGATION_POINT) {

			const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

			m_navPoints.push_back(gameObject);

			if (navComponent->type() == NavigationObjectType::WAYPOINT) {
				m_wayPoints.push_back(gameObject);
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
	//m_currentState = _determineState();
	m_currentState = BrainState::PATROL;

	switch (m_currentState) {

		case BrainState::PATROL:
			_doPatrol();
			break;
		default:
			_doPatrol();
			break;
	}
	

	//Test
	const auto& attachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	const auto& eye = attachmentComponent->getAttachment("DRONE_EYE");
	const auto& eyeActionComponent = eye->gameObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	auto direction = util::generateRandomNumber(1, 100);
	if (direction < 90) {
		eyeActionComponent->performRotateAction(0);
	}
	else {
		eyeActionComponent->performRotateAction(25);
	}

	


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

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	// Get the closest waypoint to your current position
	if (m_targetDestination.has_value() == false) {
		m_targetDestination = getClosestNavPoint(transform->getCenterPosition(), NavigationObjectType::WAYPOINT);
	}

	//Have we reached the current target destination.
	//If so, then get the next waypoint destination
	const auto& targetDestination = m_targetDestination->lock();

	if (calculateDistance(parent()->getCenterPosition(), targetDestination->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {

		//ToDo:Pass in an enum to tell this function how to determine next distination
		// for example, it could be the last know enemy location. Pass it in from navigate( targetType)
		m_targetDestination = getNextTargetDestination();

		//Clear out the visited itermin nav points now that we've onto
		//a new fresh target destination
		m_tempVisitedNavPoints.clear();

	}

	navigate();

}


void DroneBrainComponent::_doAlert()
{
}

void DroneBrainComponent::_doPursue()
{



}

void DroneBrainComponent::_doEngage()
{
}

std::weak_ptr<GameObject> DroneBrainComponent::getClosestNavPoint(SDL_FPoint thisPosition, int navType)
{

	//For each navigation item, calculate the distance from the given position
	float shortest{};
	std::weak_ptr<GameObject> closestWayPoint{};

	for (const auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT]) {

		if (gameObject->idTag() == IdTag::NAVIGATION_POINT) {

			const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
			if (navComponent->type() == navType) {

				const auto& transform = gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
				auto navPosition = transform->getCenterPosition();

				//distance calculation
				auto distance = std::powf((thisPosition.x - navPosition.x), 2) + std::powf((thisPosition.y - navPosition.y), 2);
				distance = std::sqrtf(distance);

				if (shortest == 0. || shortest > distance) {
					shortest = distance;
					closestWayPoint = gameObject;
				}
			}
		}

	}

	return closestWayPoint;
}

void DroneBrainComponent::navigate()
{

	//If we do not have an interim destination then we are off the nav path so get to the nearest one
	if (m_interimDestination.has_value() == false) {
		m_interimDestination = getClosestNavPoint(parent()->getCenterPosition(), NavigationObjectType::TRANSIT_POINT);
	}

	//If we have reached the interim destination then find the next possible interim destination that gets us
	//closest to the target dstination
	//Its possible that the interim destination that we find IS the actual target destination we're trying to get to
	const auto& interimDestination = m_interimDestination->lock();
	if (calculateDistance(parent()->getCenterPosition(), interimDestination->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {

		//Now that we have hit this interim nav point, add it to a list of visted nav points
		//so that we can avoid these while trying to navigate to the ultimate target destination
		m_tempVisitedNavPoints.push_back(interimDestination);

		m_interimDestination = getNextinterimDestination();

	}

	//Execute the move actions to get us closer to the interim destination point
	std::shared_ptr interim = m_interimDestination->lock();
	const auto& interimNavComponent = interim->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	b2Vec2 trajectory{};
	trajectory.x = interimNavComponent->getCenterPosition().x - parent()->getCenterPosition().x;
	trajectory.y = interimNavComponent->getCenterPosition().y - parent()->getCenterPosition().y;

	trajectory.Normalize();

	auto action = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	action->performMoveAction(trajectory);

	//Possibly adjust movement with small impulse moves to avoid brushing obstacles
	_applyAvoidanceMovement();

	//Set the angle to point towards the next nav point using the trajectory we calculated above
	_rotateTowards(trajectory);


}

float DroneBrainComponent::calculateDistance(SDL_FPoint location1, SDL_FPoint location2)
{

	auto distance = std::powf((location1.x - location2.x), 2) + std::powf((location1.y - location2.y), 2);
	distance = std::sqrtf(distance);

	return distance;
}


std::shared_ptr<GameObject> DroneBrainComponent::getNextTargetDestination()
{

	const auto& currentTarget = m_targetDestination->lock();
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

	auto newDestination = m_wayPoints[m_currentWaypointIndex]->lock();
	return newDestination;

}

/*
Find the accessible iterim point that is the closest to the current target waypoint
*/
std::shared_ptr<GameObject> DroneBrainComponent::getNextinterimDestination()
{
	std::shared_ptr<GameObject> foundNavGameObject;

	const auto& target = m_targetDestination->lock();
	const auto& targetTransformComponent = target->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	const auto& currentInterim = m_interimDestination->lock();
	const auto& interimNavComponent = currentInterim->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);


	std::optional<float> shortestDistance{};
	std::shared_ptr<GameObject>shortestDistanceObject{};
	for (auto& navPoint : interimNavComponent->accessibleNavObjects()) { //are these in order of distance? I thin kso...

		if (existsInAlreadyVistedNavList(navPoint) == false) {
			const auto& navPointTemp = navPoint.lock();
			const auto& navPointTransformComponent = navPointTemp->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

			//Calulate the distance from this accessible nav point and the target waypoint
			float distance = calculateDistance(navPointTransformComponent->getCenterPosition(), targetTransformComponent->getCenterPosition());

			if (shortestDistance.has_value() == false || shortestDistance.value() > distance) {

				shortestDistance = distance;
				shortestDistanceObject = navPointTemp;

			}
		}
	}


	return shortestDistanceObject;


}

bool DroneBrainComponent::existsInAlreadyVistedNavList(std::weak_ptr<GameObject> navPoint)
{
	bool found{ false };

	for (const auto vistedPoint : m_tempVisitedNavPoints) {

		if (navPoint.lock() == vistedPoint.lock()) {
			found = true;
		}
	}

	return found;
}

void DroneBrainComponent::_rotateTowards(b2Vec2 targetPoint)
{

	auto physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	auto currentAngle = physicsComponent->angle();

	auto angle = atan2(targetPoint.y, targetPoint.x);
	angle = util::normalizeRadians(angle);
	auto angleDegrees = util::radiansToDegrees(angle);
	float rotationVelocity{ 0 };

	auto action = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	auto vitality = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	if (m_targetAngle.has_value() == true) {


		if ((m_targetAngle.value() - currentAngle) < 0.0) {
			rotationVelocity = vitality->rotateSpeed() * -1;
		}
		else {
			rotationVelocity = vitality->rotateSpeed();
		}

		auto difference = abs(m_targetAngle.value() - currentAngle);

		//Once the angle is very close then set the angle directly
		if (difference < 0.5) {
			parent()->setAngleInRadians(m_targetAngle.value());
			m_targetAngle = angle;
			action->performRotateAction(0);
		}
		else {
			action->performRotateAction(rotationVelocity);
		}
	}
	else {
		m_targetAngle = angle;
		action->performRotateAction(0);

	}


}

void DroneBrainComponent::_applyAvoidanceMovement()
{

	const auto& physics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//Check all sensor detected objects and see if any are close enough to prompt a movement adjustment
	DebugPanel::instance().addItem("Distance Hit: ", "");
	for (const auto& intersectionItem : m_seenObjects) {


		if (intersectionItem.fraction * 25 < 5) {

			DebugPanel::instance().addItem("Seen Object Distance: ", intersectionItem.fraction * 25, 5);
			DebugPanel::instance().addItem("Distance Hit: ", "HIT");

			DebugPanel::instance().addItem("PULSE: ", "");
			if (intersectionItem.gameObject->id().starts_with("WALL")) {
				DebugPanel::instance().addItem("PULSE: ", "PULSE");
				DebugPanel::instance().addItem("TrajectoryX: ", intersectionItem.normal.x, 1);
				DebugPanel::instance().addItem("TrajectoryY: ", intersectionItem.normal.y, 1);
				auto trajectory = intersectionItem.normal;
				physics->applyImpulse(300000, trajectory);




				const auto& transform = intersectionItem.gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
				b2Vec2 centerPosition = { transform->getCenterPosition().x , transform->getCenterPosition().y };

				b2Vec2 begin = centerPosition;
				b2Vec2 end = trajectory;

				auto lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
				auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
				lineTransform->setLine(begin, end);




			}

		}

	}

}
