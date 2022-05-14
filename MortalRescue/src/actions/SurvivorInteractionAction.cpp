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
		brainComponent->followMe(interactingObject);
	}
	if (keyScanCode == SDL_SCANCODE_R) {
		brainComponent->stay();
	}


}
