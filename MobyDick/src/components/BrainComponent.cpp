#include "BrainComponent.h"

#include <box2d/box2d.h>

#include "../game.h"
#include "../DebugPanel.h"
#include "../RayCastCallBack.h"
#include "../BrainAABBCallback.h"
#include "../Camera.h"

#include <math.h>
#include <random>
#include "glm/glm.hpp"

extern std::unique_ptr<Game> game;

BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value componentJSON)
{

	m_sensorLength = componentJSON["sensorLength"].asInt();
	m_sensorOffset = componentJSON["sensorOffset"].asInt();
	m_sensorCount = componentJSON["sensorCount"].asInt();
	m_sightSensorSize = componentJSON["sightSensorSize"].asInt();

}

BrainComponent::~BrainComponent()
{

}

void BrainComponent::update()
{

	_updateSensorInput();

}

void BrainComponent::postInit()
{

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

	//top
	game->renderer()->addLine(topLeft, topRight, lineColor);
	//right
	game->renderer()->addLine(topRight, botRight, lineColor);
	//bottom
	game->renderer()->addLine(botRight, botLeft, lineColor);
	//left
	game->renderer()->addLine(botLeft, topLeft, lineColor);

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

			if (rayHitObject.gameObject->hasTrait(TraitTag::barrier)) {
				clearPath = false;
				break;
			}
		}

		RayCastCallBack::instance().reset();
	}

	return clearPath;
}
