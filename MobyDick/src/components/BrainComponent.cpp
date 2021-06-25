#include "BrainComponent.h"

#include <box2d/box2d.h>

#include "../game.h"
#include "../DebugPanel.h"
#include "../RayCastCallBack.h"

#include <math.h>
#include <random>


BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value definitionJSON)
{

	Json::Value brainComponentJSON = definitionJSON["brainComponent"];

	m_sensorLength = brainComponentJSON["sensorLength"].asInt();
	m_sensorOffset = brainComponentJSON["sensorOffset"].asInt();
	m_sensorCount = brainComponentJSON["sensorCount"].asInt();


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

void BrainComponent::_updateSensorInput()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	auto centerPosition = parent()->getCenterPosition();

	/*
	TODO:Create get angle funtion
	*/
	//float currentParentAngle = util::degreesToRadians(transform->angle());

	//Send out sensor rays 
	float angleStagger = (float)360 / (float)m_sensorCount;
	m_currentSensorIteration++;
	if (m_currentSensorIteration == m_sensorCount) {

		RayCastCallBack::instance().reset();
		m_currentSensorIteration = 1;
	}
	
	auto sensorAngle = m_currentSensorIteration * angleStagger;

	auto angleAdj = util::degreesToRadians(sensorAngle);
	//auto angle = util::normalizeRadians(currentParentAngle + angleAdj);
	auto angle = util::normalizeRadians(angleAdj);

	b2Vec2 direction{ cos(angle) * (sensorLength()) , sin(angle) * (sensorLength()) };
	b2Vec2 offset = { cos(angle) * (sensorOffset()) , sin(angle) * (sensorOffset()) };

	b2Vec2 begin = { centerPosition.x + offset.x, centerPosition.y + offset.y};
	b2Vec2 end = { begin.x + direction.x, begin.y + direction.y };

	util::toBox2dPoint(begin);
	util::toBox2dPoint(end);

	parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);

	util::toRenderPoint(begin);
	util::toRenderPoint(end);

	auto lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
	auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	lineTransform->setLine(begin, end);

	//Store all of the objects that were hit by our raycast calls
	m_seenObjects = RayCastCallBack::instance().intersectionItems();

}

