#include "OnetimeButtonInteraction.h"
#include <iostream>
#include "game.h"
#include "../particleEffects/GameParticleEffects.h"
#include "SoundManager.h"

extern std::unique_ptr<Game> game;

void OnetimeButtonInteraction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode)
{
	std::string buttonTargetObjectName = interactionObject->name() + "_TARGET";

	auto bottonTargetObjects = interactionObject->parentScene()->getGameObjectsByName(buttonTargetObjectName);
	assert(!bottonTargetObjects.empty() && "GameObject wasnt found!");

	const auto& animationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	//Animate the button
	animationComponent->animate(ANIMATION_DEAD, ANIMATE_ONE_TIME);

	for (auto& targetObject : bottonTargetObjects) {

		if (targetObject->renderDisabled() == true && targetObject->physicsDisabled() == true) {
			targetObject->enableRender();
			targetObject->enablePhysics();

		}
		else {
			targetObject->disableRender();
			targetObject->disablePhysics();
		}

	}

	SoundManager::instance().playSound("SFX_BUTTON_BROKEN_1");
	SoundManager::instance().playSound("SFX_DOOR_ACTIVATE_1");

	//Disable this button sinc eits been used its onetime
	interactionObject->disablePhysics();
	interactionObject->setColor(Colors::RED);


}
