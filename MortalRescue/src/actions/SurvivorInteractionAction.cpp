#include "SurvivorInteractionAction.h"
#include "../components/SurvivorBrainComponent.h"
#include <iostream>
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

void SurvivorInteractionAction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyScanCode)
{
	const auto& brainComponent = interactionObject->getComponent<SurvivorBrainComponent>(ComponentTypes::BRAIN_COMPONENT);

	if (keyScanCode == SDL_SCANCODE_E) {

		//If we're storing this interacting gameObject in another game objects brain, then we need the shared_ptr version of the pointer
		//in case this interacting game object is deleted
		auto gameObjectSharedPtr = interactingObject->parentScene()->getGameObject(interactingObject->name());
		assert(gameObjectSharedPtr.has_value() && "GameObject wasnt found!");

		brainComponent->followMe(gameObjectSharedPtr.value());
	}
	if (keyScanCode == SDL_SCANCODE_R) {
		brainComponent->stay();
	}


}
