#include "DoorEnterAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorEnterAction::perform(GameObject* playerObject, GameObject* doorObject)
{

	const auto& physicsComponent = playerObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//build name for the door destination object 
	std::string doorDestination = doorObject->name() + "_EXIT";

	//Find this doors destination object
	const auto& destination = doorObject->parentScene()->getFirstGameObjectByName(doorDestination);
	if (destination.has_value()) {

		//Get location and use it as warp destination
		b2Vec2 destinationLocation = { destination.value()->getCenterPosition().x, destination.value()->getCenterPosition().y };
		util::toBox2dPoint(destinationLocation);
		physicsComponent->setTransform(destinationLocation);

	}


}
