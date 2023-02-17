#include "DoorInteractionAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorInteractionAction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyScanCode)
{

	if (keyScanCode == SDL_SCANCODE_E) {

		const auto& physicsComponent = interactingObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

		//build name for the door destination object 
		std::string doorDestination = interactionObject->name() + "_EXIT";

		//Find this doors partner
		const auto& destination = interactionObject->parentScene()->getFirstGameObjectByName(doorDestination);
		if (destination.has_value()) {

			//Get location and use it as warp destination
			b2Vec2 destinationLocation = { destination.value()->getCenterPosition().x, destination.value()->getCenterPosition().y};
			util::toBox2dPoint(destinationLocation);
			physicsComponent->setTransform(destinationLocation);

		}

	}
	else if (keyScanCode == SDL_SCANCODE_R) {

	}
	else if (keyScanCode == SDL_SCANCODE_W) {

		util::sendSceneEvent(SCENE_ACTION_ADD, "SCENE_PLAYER_DEATH");


	}


}
