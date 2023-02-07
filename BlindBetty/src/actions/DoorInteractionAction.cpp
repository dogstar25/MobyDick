#include "DoorInteractionAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorInteractionAction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyScanCode)
{

	if (keyScanCode == SDL_SCANCODE_E) {

		const auto& physicsComponent = interactingObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);


		//Find this doors partner
		const auto& doors = interactionObject->parentScene()->getGameObjectsByName(interactionObject->name());
		for (const auto& door : doors) {

			//If we match on name, and this isnt the object itself, then its our match/partner
			if (door->name() == interactionObject->name() && door.get() != interactionObject) {

				//Get location and use it as warp destination
				b2Vec2 destination = { door->getCenterPosition().x, door->getCenterPosition().y };
				util::toBox2dPoint(destination);

				physicsComponent->setTransform(destination);

			}

		}

	}
	else if (keyScanCode == SDL_SCANCODE_R) {

	}
	else if (keyScanCode == SDL_SCANCODE_W) {

		util::sendSceneEvent(SCENE_ACTION_ADD, "SCENE_PLAYER_DEATH");


	}


}
