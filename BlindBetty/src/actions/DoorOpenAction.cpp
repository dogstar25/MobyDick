#include "DoorOpenAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorOpenAction::perform(GameObject* doorObject)
{

	const auto& doorAnimationComponent = doorObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	auto doorState = doorAnimationComponent->currentAnimationState();

	GameObject* partnerDoor{};

	//Get this doors matching partner to sets its state as well
	const auto& doors = doorObject->parentScene()->getGameObjectsByName(doorObject->name());
	for (const auto& door : doors) {
		if (door.get() != doorObject) {

			partnerDoor = door.get();
			break;

		}
	}

	doorAnimationComponent->animate(AnimationState::OPEN, ANIMATE_ONE_TIME);
	doorAnimationComponent->setDefaultAnimationState(AnimationState::OPENED);
	partnerDoor->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT)->setCurrentAnimationState(AnimationState::CLOSED);

}

