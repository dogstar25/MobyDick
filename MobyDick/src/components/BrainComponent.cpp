#include "BrainComponent.h"

#include <box2d/box2d.h>

#include "../game.h"


BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value definitionJSON)
{

	Json::Value brainComponentJSON = definitionJSON["brainComponent"];


	//Get the waypoints
	m_waypoints = Level::instance().waypoints();




}

BrainComponent::~BrainComponent()
{

}

void BrainComponent::postInit(const std::array <std::vector<std::shared_ptr<GameObject>>, MAX_GAMEOBJECT_LAYERS>& gameObjectCollection)
{

}


void BrainComponent::update()
{

	_updateSightInput();
	_updateSensorInput();

	_doPatrol();


}

void BrainComponent::_updateSightInput()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	b2Vec2 position = { transform->getCenterPosition().x / 25, transform->getCenterPosition().y / 25 };
	const b2Vec2 changePosition = b2Vec2(10, 10);
	b2Vec2 newPosition = position + changePosition;

	//auto lineObject = Game::instance().addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
	//auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	//b2Vec2 begin = { position.x * 25 , position.y * 25 };
	//b2Vec2 end = { newPosition.x * 25 , newPosition.y * 25 };
	//lineTransform->setLine(begin, end);


	Game::instance().physicsWorld()->RayCast(&m_b2RayCastCallback, position, newPosition);


}

void BrainComponent::_updateSensorInput()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	const b2Vec2 centerPosition = { transform->getCenterPosition().x / 25, transform->getCenterPosition().y / 25 };

	float sensorIncrement = 18;
	//Create a sensor raycast in every direction full 360%
	for (int rayCount = 0; rayCount <= sensorIncrement; rayCount++) {

		float angle = util::degreesToRadians(rayCount * sensorIncrement);
		b2Vec2 direction{ transform->angle() + cos(angle) * 10, transform->angle() + sin(angle) * 10 };

		auto lineObject = Game::instance().addGameObject("PRIMITIVE_LINE", LAYER_MAIN, -1, -1);
		auto lineTransform = lineObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

		//convert back to world coordinates to draw
		/*centerPosition *= 25;
		direction *= 25;*/

		b2Vec2 begin = { centerPosition.x * 25, centerPosition.y*25};
		b2Vec2 end = { centerPosition.x * 25 + direction.x * 25, centerPosition.y * 25 + direction.y * 25 };
		lineTransform->setLine(begin, end);

	}



}

void BrainComponent::_doPatrol()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	b2Vec2 position = { transform->getCenterPosition().x / 25, transform->getCenterPosition().y / 25 };
	const b2Vec2 changePosition = b2Vec2(10, 10);
	b2Vec2 newPosition = position + changePosition;

	/*auto& waypoint1 = m_waypoints[0];
	b2Vec2 tragectory = waypoint1.point - position;*/

	auto action = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	//action->performMoveAction(1, 1);


}

void BrainComponent::_doAlert()
{
}

void BrainComponent::_doPursue()
{
}

void BrainComponent::_doEngage()
{
}
