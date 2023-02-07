#include "ActorWarpAction.h"

#include "../GameObject.h"

void ActorWarpAction::perform(GameObject* interactingObject, GameObject* interactionObject, b2Vec2 destination)
{
	const auto& physicsComponent = interactingObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	auto b2Destination = util::toBox2dPoint(destination);

	physicsComponent->setTransform(b2Destination);

}
