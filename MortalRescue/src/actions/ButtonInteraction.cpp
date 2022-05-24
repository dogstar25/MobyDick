#include "ButtonInteraction.h"
#include <iostream>
#include "game.h"

extern std::unique_ptr<Game> game;

void ButtonInteraction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode)
{
	std::string buttonTargetObjectName = interactionObject->name() + "_TARGET";
	GameObject* bottonTargetObject = game->getGameObject(buttonTargetObjectName);
	const auto& animationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (bottonTargetObject->disabled() == true) {
		bottonTargetObject->enable();
		bottonTargetObject->show();
		animationComponent->animate(ANIMATION_ACTIVE, ANIMATE_CONTINUOUS);
	}
	else {
		bottonTargetObject->disable(true);
		bottonTargetObject->hide();
		animationComponent->animate(ANIMATION_IDLE, ANIMATE_CONTINUOUS);
	}

	
}
