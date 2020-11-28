#include "BrainComponent.h"

#include <box2d/box2d.h>

#include "../game.h"

BrainComponent::BrainComponent()
{
}

BrainComponent::BrainComponent(Json::Value definitionJSON)
{

	Json::Value brainComponentJSON = definitionJSON["brainComponent"];





}

BrainComponent::~BrainComponent()
{

}

void BrainComponent::update()
{

	auto transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	b2Vec2 position = { transform->getCenterPosition().x, transform->getCenterPosition().y };
	const b2Vec2 changePosition = b2Vec2(1025, 1025);
	b2Vec2 newPosition = position + changePosition;
	
	auto x = 1025 / 25;
	auto y = 1025 / 25;

	//auto particleEmitterObject = Game::instance().addGameObject("PARTICLE_X_EMITTER", LAYER_TEXT, (int)x, (int)y);



	Game::instance().physicsWorld()->RayCast(&m_b2RayCastCallback, position, changePosition);


}