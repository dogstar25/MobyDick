#include "ToggleButtonInteraction.h"
#include <iostream>
#include "game.h"
#include "SoundManager.h"

extern std::unique_ptr<Game> game;

void ToggleButtonInteraction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode)
{
	std::string buttonTargetObjectName = interactionObject->name() + "_TARGET";

	auto bottonTargetObjects = interactionObject->parentScene()->getGameObjectsByName(buttonTargetObjectName);
	assert(!bottonTargetObjects.empty() && "GameObject wasnt found!");

	const auto& animationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	//Animate the button
	animationComponent->animate(ANIMATION_ACTIVE, ANIMATE_ONE_TIME);

	for (auto& targetObject : bottonTargetObjects) {

		if (targetObject->renderDisabled() == true && targetObject->physicsDisabled() == true) {
			targetObject->enableRender();
			targetObject->enablePhysics();
			
		}
		else if (targetObject->updateDisabled() == true) {
			targetObject->enableUpdate();
		}
		else {
			targetObject->disableRender();
			targetObject->disablePhysics();
		}

		//If the object being toggled has a toggle sound then play it
		if (targetObject->hasComponent(ComponentTypes::SOUND_COMPONENT) == true) {

			const auto& soundComponent = targetObject->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
			soundComponent->playSound("TOGGLE_SOUND");
		}

	}


	//SoundManager::instance().playSound("SFX_BUTTON_WORKING_1");

	//Play Sound
	if (interactionObject->hasComponent(ComponentTypes::SOUND_COMPONENT) == true) {
		const auto& soundComponent = interactionObject->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);
		soundComponent->playSound("USE_SOUND");
	}


	//Put this in the gameObject disable physics function and play the "DISABLE_SOUND" if it exists for the object
	//SoundManager::instance().playSound("SFX_DOOR_ACTIVATE_1");
	
	
}
