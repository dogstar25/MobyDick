#include "ButtonInteraction.h"
#include <iostream>
#include "game.h"

extern std::unique_ptr<Game> game;

void ButtonInteraction::perform(GameObject* gameObject)
{
	std::string buttonTargetObjectName = gameObject->name() + "_TARGET";
	GameObject* bottonTargetObject = game->getGameObject(buttonTargetObjectName);
	const auto& animationComponent = gameObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (bottonTargetObject->disabled() == true) {
		bottonTargetObject->enable();
		animationComponent->animate(ANIMATION_ACTIVE, ANIMATE_CONTINUOUS);
	}
	else {
		bottonTargetObject->disable(true);
		animationComponent->animate(ANIMATION_IDLE, ANIMATE_CONTINUOUS);
	}

	
}
