#include "InteractAction.h"

#include "../components/PhysicsComponent.h"
#include <memory>

void InteractAction::perform(GameObject* gameObject, SDL_Scancode keyScanCode)
{

		for (const auto& touchingObject : gameObject->getTouchingObjects()) {

			if (touchingObject.second.expired() == false && touchingObject.second.lock()->hasTrait(TraitTag::interactive) ) {

				GameObject* interactiveObject = touchingObject.second.lock().get();
				const auto& interactiveObjectActionComponent = interactiveObject->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
				//Is the interactingObject(player) pointing at this interactive object?
				if (gameObject->isPointingAt(interactiveObject->getCenterPosition())) {
					const auto& action = interactiveObjectActionComponent->getAction(ACTION_INTERACTION);
					action->perform(gameObject, interactiveObject, keyScanCode);
				}
			}
		}

}