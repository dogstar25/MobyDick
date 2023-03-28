#include "DoorOpenAction.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void DoorOpenAction::perform(GameObject* doorObject)
{

	const auto& doorAnimationComponent = doorObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	auto doorState = doorAnimationComponent->currentAnimationState();

	std::optional<GameObject*> partnerDoor{};

	//Get this doors matching partner to sets its state as well
	//It's possibvle that a partner door does not exist
	const auto& doors = doorObject->parentScene()->getGameObjectsByName(doorObject->name());
	for (const auto& door : doors) {
		if (door.get() != doorObject) {

			partnerDoor = door.get();
			break;

		}
	}

	//animate the open
	doorAnimationComponent->animate(AnimationState::OPEN, ANIMATE_ONE_TIME);
	doorAnimationComponent->setDefaultAnimationState(AnimationState::OPENED);

	//disable the collision
	doorObject->disableCollision();

	//If there is a partner door, animate state to opened and disabled its collision
	if (partnerDoor.has_value()) {
		partnerDoor.value()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT)->setCurrentAnimationState(AnimationState::OPENED);
		partnerDoor.value()->disableCollision();
	}


}

