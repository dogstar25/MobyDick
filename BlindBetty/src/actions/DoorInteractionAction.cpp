#include "DoorInteractionAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorInteractionAction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyScanCode)
{

	if (keyScanCode == SDL_SCANCODE_E) {


		const auto& doorAnimationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
		auto doorState = doorAnimationComponent->currentAnimationState();

		GameObject* partnerDoor{};

		//Get this doors matching partner to sets its state as well
		const auto& doors = interactionObject->parentScene()->getGameObjectsByName(interactionObject->name());
		for (const auto& door : doors) {
			if (door.get() != interactionObject) {

				partnerDoor = door.get();
				
			}
		}

		if (doorState == AnimationState::CLOSED) {

			doorAnimationComponent->animate(AnimationState::OPEN, ANIMATE_ONE_TIME);
			doorAnimationComponent->setDefaultAnimationState(AnimationState::OPENED);
			partnerDoor->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT)->setCurrentAnimationState(AnimationState::OPENED);
			

		}
		else if (doorState == AnimationState::OPENED) {
			doorAnimationComponent->animate(AnimationState::CLOSE, ANIMATE_ONE_TIME);
			doorAnimationComponent->setDefaultAnimationState(AnimationState::CLOSED);
			partnerDoor->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT)->setCurrentAnimationState(AnimationState::CLOSED);
		}


	}
	else if (keyScanCode == SDL_SCANCODE_W) {

		const auto& physicsComponent = interactingObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

		//build name for the door destination object 
		std::string doorDestination = interactionObject->name() + "_EXIT";

		//Find this doors partner
		const auto& destination = interactionObject->parentScene()->getFirstGameObjectByName(doorDestination);
		if (destination.has_value()) {

			//Get location and use it as warp destination
			b2Vec2 destinationLocation = { destination.value()->getCenterPosition().x, destination.value()->getCenterPosition().y };
			util::toBox2dPoint(destinationLocation);
			physicsComponent->setTransform(destinationLocation);

		}

	}


}
