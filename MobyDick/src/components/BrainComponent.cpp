#include "BrainComponent.h"


#include "../game.h"
#include "../RayCastCallBack.h"


extern std::unique_ptr<Game> game;

BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value componentJSON)
{

	std::stringstream test; 
	test << componentJSON.toStyledString();
	std::cout << test.str();

	m_sensorLength = componentJSON["sensorLength"].asInt();
	m_sensorOffset = componentJSON["sensorOffset"].asInt();
	m_sensorCount = componentJSON["sensorCount"].asInt();
	m_sightSensorSize = componentJSON["sightSensorSize"].asInt();

}

BrainComponent::~BrainComponent()
{

}

void BrainComponent::_doIdle()
{

	stopMovement();

}

bool BrainComponent::_doDispatch()
{
	bool dispatchPointReached{ false };

	//Have we reached the current target destination.
	//If so, then get the next waypoint destination
	if (util::calculateDistance(parent()->getCenterPosition(),
		m_targetDestination.value()->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {

		//Clear out the visited itermin nav points now that we've onto
		//a new fresh target destination
		m_tempVisitedNavPoints.clear();
		dispatchPointReached = true;
		m_currentState = BrainState::IDLE;
	}
	else {
		navigate();
	}

	return dispatchPointReached;

}

void BrainComponent::dispatch(SDL_FPoint destination)
{
	m_currentState = BrainState::DISPATCH;
	setTargetDestination(destination);
}

void BrainComponent::dispatch()
{
	m_currentState = BrainState::DISPATCH;
	
}


void BrainComponent::setTargetDestination(SDL_FPoint destination)
{

	//Find the navigation point closest to the given destination location given
	m_targetDestination = getClosestNavPoint(destination, NavigationObjectType::UNSPECIFIED);


}

void BrainComponent::update()
{

	_updateSensorInput();

	switch (m_currentState) {

	case BrainState::DISPATCH:
		_doDispatch();
		break;
	default:
		_doIdle();
		break;
	}


}

void BrainComponent::postInit()
{

	//Get all NavPoints
	for (const auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT]) {

		if (gameObject->hasTrait(TraitTag::navigation)) {

			const auto& navComponent = gameObject->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);

			m_navPoints.push_back(gameObject.get());

		}
	}

}



void BrainComponent::_updateSensorInput()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	auto centerPosition = parent()->getCenterPosition();

	m_detectedObjects.clear();
	m_seenObjects.clear();

	b2Vec2 centerB2 = { centerPosition.x, centerPosition.y};
	util::toBox2dPoint(centerB2);

	b2AABB aabb;
	aabb.lowerBound = b2Vec2(centerB2.x - m_sightSensorSize, centerB2.y - m_sightSensorSize);
	aabb.upperBound = b2Vec2(centerB2.x + m_sightSensorSize, centerB2.y + m_sightSensorSize);
	
	//Draw the sight sensor
	glm::vec2 topLeft = { aabb.lowerBound.x, aabb.lowerBound.y };
	glm::vec2 topRight = { aabb.upperBound.x, aabb.lowerBound.y };
	glm::vec2 botRight = { aabb.upperBound.x, aabb.upperBound.y };
	glm::vec2 botLeft = { aabb.lowerBound.x, aabb.upperBound.y };
	glm::vec4 lineColor{ 255,255,255,255 };
	util::toRenderPoint(topLeft);
	util::toRenderPoint(topRight);
	util::toRenderPoint(botRight);
	util::toRenderPoint(botLeft);

	topLeft -= glm::vec2{Camera::instance().frame().x, Camera::instance().frame().y};
	topRight -= glm::vec2{ Camera::instance().frame().x, Camera::instance().frame().y };
	botRight -= glm::vec2{ Camera::instance().frame().x, Camera::instance().frame().y };
	botLeft -= glm::vec2{ Camera::instance().frame().x, Camera::instance().frame().y };

	////top
	//game->renderer()->addLine(topLeft, topRight, lineColor);
	////right
	//game->renderer()->addLine(topRight, botRight, lineColor);
	////bottom
	//game->renderer()->addLine(botRight, botLeft, lineColor);
	////left
	//game->renderer()->addLine(botLeft, topLeft, lineColor);

	//Make the AABB query
	parent()->parentScene()->physicsWorld()->QueryAABB(&BrainAABBCallback::instance(), aabb);

	//Loop through all of the found objects and store the ones that we care about that we have a direct line
	//of sight to
	for (BrainAABBFoundObject detectedObject : BrainAABBCallback::instance().foundObjects()) {

		m_detectedObjects.push_back(detectedObject);

		if (detectedObject.gameObject->hasTrait(TraitTag::player) ||
			detectedObject.gameObject->hasTrait(TraitTag::navigation) ||
			detectedObject.gameObject->hasTrait(TraitTag::navigation)) {

			if (_hasLineOfSight(detectedObject) == true) {

				m_seenObjects.push_back(detectedObject);

			}
		}
	}

	BrainAABBCallback::instance().reset();

}

/*
* This is a ray from a detected object to the Brain Owner
* If no barrier object was found along this ray, then there
* is a clear line of sight
*/
bool BrainComponent::_hasLineOfSight(BrainAABBFoundObject& detectedObject)
{
	//Get parents position
	auto parentPosition = parent()->getCenterPosition();

	//Cast a ray from this object to the brain parent to see if we have a direct line of sight
	auto objectPosition = detectedObject.gameObject->getCenterPosition();
	b2Vec2 begin = { objectPosition.x, objectPosition.y };
	b2Vec2 end = { parentPosition.x, parentPosition.y };

	util::toBox2dPoint(begin);
	util::toBox2dPoint(end);

	b2Vec2 distance = end - begin;

	bool clearPath{ true };

	// If the distance is zero, 
	//then this object is right on top of the Brain owner so assume its a seen object
	//and dont call the raycast because box2d will blow up if distance is zero
	if (distance.LengthSquared() > 0.0f) {

		//Cast the ray, storing all intersected objects
		parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);

		//Loop through all objects hit between the brain owner and the detected object
		//If there is a clear line of sight then store it in seenObjects
		//We must sort the raycast hit objects by distance because they are not guarenteed to return in
		//distance order
		std::sort(RayCastCallBack::instance().intersectionItems().begin(),
			RayCastCallBack::instance().intersectionItems().end(),
			intersection_sort_compare());

		for (BrainRayCastFoundItem rayHitObject : RayCastCallBack::instance().intersectionItems()) {

			//Is this a barrier and also NOT its own body
			if (rayHitObject.gameObject->hasTrait(TraitTag::barrier) && rayHitObject.gameObject != parent()) {
				clearPath = false;
				break;
			}
		}

		RayCastCallBack::instance().reset();
	}

	return clearPath;
}

void BrainComponent::navigate()
{

	//If we do not have an interim destination then we are off the nav path so get to the nearest one
	if (m_interimDestination.has_value() == false) {
		m_interimDestination = getClosestSeenNavPoint(parent()->getCenterPosition(), NavigationObjectType::UNSPECIFIED);
	}

	//If we have reached the interim destination then find the next possible interim destination that gets us
	//closest to the target dstination
	//Its possible that the interim destination that we find IS the actual target destination we're trying to get to
	//std::optional<std::shared_ptr<GameObject>> nextInterimDestination{ m_interimDestination };
	bool alreadyAtClosestInterimToTargetDestination = false;

	//Has the drone reached the interim destination
	if (util::calculateDistance(parent()->getCenterPosition(), m_interimDestination.value()->getCenterPosition())
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
		stopMovement();
	}
	else {
		executeMove();
	}

}

void BrainComponent::executeMove()
{
	GameObject* interim = m_interimDestination.value();
	const auto& interimNavComponent = interim->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	b2Vec2 trajectory{};
	trajectory.x = interimNavComponent->getCenterPosition().x - parent()->getCenterPosition().x;
	trajectory.y = interimNavComponent->getCenterPosition().y - parent()->getCenterPosition().y;

	trajectory.Normalize();

	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
	moveAction->perform(parent(), trajectory);

	//Possibly adjust movement with small impulse moves to avoid brushing obstacles
	//_applyAvoidanceMovement();

	//Set the angle to point towards the next nav point using the trajectory we calculated above
	//_rotateTowards(trajectory, parent());


}

void BrainComponent::stopMovement()
{
	b2Vec2 trajectory{ 0,0 };
	float angularVelocity{ 0. };

	auto actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	if (actionComponent->getAction(ACTION_MOVE)) {
		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), trajectory);
	}

	if (actionComponent->getAction(ACTION_ROTATE)) {
		const auto& moveAction = actionComponent->getAction(ACTION_ROTATE);
		moveAction->perform(parent(), angularVelocity);
	}


}


/*
Find the accessible iterim point that is the closest to the current target
*/
std::optional<GameObject*> BrainComponent::getNextinterimDestination()
{
	const auto& target = m_targetDestination.value();
	const auto& targetTransformComponent = target->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	const auto& currentInterim = m_interimDestination.value();
	const auto& interimNavComponent = currentInterim->getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
	const auto& interimTransformComponent = currentInterim->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);


	std::optional<float> shortestDistance{};
	std::optional<GameObject*>shortestDistanceObject{};

	//Are we already at the closest nav point possible
	if (util::calculateDistance(interimTransformComponent->getCenterPosition(),
		targetTransformComponent->getCenterPosition()) < DESTINATION_DISTANCE_TOLERANCE) {
		shortestDistanceObject = std::nullopt;
	}
	else {

		for (auto& navPoint : interimNavComponent->accessibleNavObjects()) {

			//we store previously visited nav points when navigating to a destination point so that we do not get stuck between 2 nav points
			//it forces us the move ahead. Certain weird map layouts coudl cause this situation
			if (_existsInAlreadyVistedNavList(navPoint.get()) == false) {
				const auto& navPointTemp = navPoint;
				const auto& navPointTransformComponent = navPointTemp->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

				//Calulate the distance from this accessible nav point and the target waypoint
				float distance = util::calculateDistance(navPointTransformComponent->getCenterPosition(), targetTransformComponent->getCenterPosition());

				if (shortestDistance.has_value() == false || shortestDistance.value() > distance) {

					shortestDistance = distance;
					shortestDistanceObject = navPointTemp.get();

				}
			}
		}

	}

	return shortestDistanceObject;

}

bool BrainComponent::_existsInAlreadyVistedNavList(GameObject* navPoint)
{
	bool found{ false };

	for (const auto vistedPoint : m_tempVisitedNavPoints) {

		if (navPoint == vistedPoint) {
			found = true;
		}
	}

	return found;
}

GameObject* BrainComponent::getClosestSeenNavPoint(SDL_FPoint targetPosition, int navType)
{

	//For each navigation item, calculate the distance from the given position
	float shortest{};
	GameObject* closestNavPoint{};

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
					closestNavPoint = seenObject.gameObject;
				}
			}

		}

	}

	//If we didnt find any navpoint that we could see, then just get closest one period and 
	// hope for the best that it wont collide with something
	if (!closestNavPoint) {

		closestNavPoint = getClosestNavPoint(targetPosition, navType);

	}

	assert(closestNavPoint != nullptr && "Brain: No navpoint was found!");

	return closestNavPoint;
}

GameObject* BrainComponent::getClosestNavPoint(SDL_FPoint targetPosition, int navType)
{
	float shortest{};
	GameObject* closestNavPoint{};

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
					closestNavPoint = gameObject.get();
				}
			}
		}

	}

	return closestNavPoint;
}

//void BrainComponent::_updateSensorInput()
//{
//
//	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//
//	auto centerPosition = parent()->getCenterPosition();
//
//	/*
//	TODO:Create get angle funtion
//	*/
//	//float currentParentAngle = util::degreesToRadians(transform->angle());
//
//	//Send out sensor rays 
//	float angleStagger = (float)360 / (float)m_sensorCount;
//	m_currentSensorIteration++;
//	if (m_currentSensorIteration == m_sensorCount) {
//
//		RayCastCallBack::instance().reset();
//		m_currentSensorIteration = 1;
//	}
//	
//	auto sensorAngle = m_currentSensorIteration * angleStagger;
//
//	auto angleAdj = util::degreesToRadians(sensorAngle);
//	//auto angle = util::normalizeRadians(currentParentAngle + angleAdj);
//	auto angle = util::normalizeRadians(angleAdj);
//
//	b2Vec2 direction{ cos(angle) * (sensorLength()) , sin(angle) * (sensorLength()) };
//	b2Vec2 offset = { cos(angle) * (sensorOffset()) , sin(angle) * (sensorOffset()) };
//
//	b2Vec2 begin = { centerPosition.x + offset.x, centerPosition.y + offset.y};
//	b2Vec2 end = { begin.x + direction.x, begin.y + direction.y };
//
//	util::toBox2dPoint(begin);
//	util::toBox2dPoint(end);
//
//	parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);
//
//	util::toRenderPoint(begin);
//	util::toRenderPoint(end);
//
//	auto lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
//	auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//	lineTransform->setLine(begin, end);
//
//	//Store all of the objects that were hit by our raycast calls
//	m_seenObjects = RayCastCallBack::instance().intersectionItems();
//
//}

//void BrainComponent::_updateSensorInput()
//{
//
//	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//	auto centerPosition = parent()->getCenterPosition();
//	float currentParentAngle = util::degreesToRadians(transform->angle());
//
//	RayCastCallBack::instance().reset();
//	m_allDetectedObjects.clear();
//	m_seenObjects.clear();
//	m_sensorCount = 90;
//	float angleStagger = (float)360 / (float)m_sensorCount;
//	for (auto i = 1; i <= m_sensorCount; i++) {
//
//		//Send out sensor rays 
//		float angle1 = i * angleStagger;
//
//		//auto sensorAngle = m_currentSensorIteration * angleStagger;
//
//		auto angleAdj = util::degreesToRadians(angle1);
//		//auto angleAdj = util::degreesToRadians(sensorAngle);
//		auto angle = util::normalizeRadians(currentParentAngle + angleAdj);
//		//auto angle = util::normalizeRadians(angleAdj);
//
//		b2Vec2 direction{ cos(angle) * (sensorLength()) , sin(angle) * (sensorLength()) };
//		b2Vec2 offset = { cos(angle) * (sensorOffset()) , sin(angle) * (sensorOffset()) };
//
//		b2Vec2 begin = { centerPosition.x + offset.x, centerPosition.y + offset.y };
//		b2Vec2 end = { begin.x + direction.x, begin.y + direction.y };
//
//		util::toBox2dPoint(begin);
//		util::toBox2dPoint(end);
//
//		parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);
//		m_allDetectedObjects.insert(m_allDetectedObjects.end(),
//			RayCastCallBack::instance().intersectionItems().begin(),
//			RayCastCallBack::instance().intersectionItems().end());
//		determineSeenObjects(RayCastCallBack::instance().intersectionItems(), m_seenObjects);
//		RayCastCallBack::instance().reset();
//
//		util::toRenderPoint(begin);
//		util::toRenderPoint(end);
//
//		auto lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
//		auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
//		lineTransform->setLine(begin, end);
//
//	}
//
//}

