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

	b2Vec2 centerPosition = { transform->getCenterPosition().x , transform->getCenterPosition().y  };

	float currentParentAngle = util::degreesToRadians(transform->angle());

	//Send out sensor rays across 180% of the front of the parent object
	RayCastCallBack::instance().reset();
	for (int i = 0; i <= 4;i++) {

		//right side
		auto angleAdj = util::degreesToRadians(22.5 * i);
		auto angle = util::normalizeRadians(currentParentAngle + angleAdj);

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

		//left side
		angleAdj = util::degreesToRadians(22.5 * i * -1) ;
		angle = util::normalizeRadians(currentParentAngle + angleAdj);

		offset = { cos(angle) * (75) , sin(angle) * (75) };
		direction = { cos(angle) * (100) , sin(angle) * (100) };
		begin = { centerPosition.x + offset.x, centerPosition.y + offset.y };
		end = { begin.x + direction.x, begin.y + direction.y };

		util::toBox2dPoint(begin);
		util::toBox2dPoint(end);

		parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), begin, end);

		util::toRenderPoint(begin);
		util::toRenderPoint(end);

		lineObject = parent()->parentScene()->addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
		lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		lineTransform->setLine(begin, end);

	}

	//Store all of the objects that were hit by our raycast calls
	m_seenObjects = RayCastCallBack::instance().intersectionItems();
	RayCastCallBack::instance().reset();

}


