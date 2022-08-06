#include "ButtonInteraction.h"
#include <iostream>
#include "game.h"

extern std::unique_ptr<Game> game;

void ButtonInteraction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode)
{
	std::string buttonTargetObjectName = interactionObject->name() + "_TARGET";

	auto bottonTargetObject = game->getGameObject(buttonTargetObjectName);
	assert(bottonTargetObject.has_value() && "GameObject wasnt found!");

	const auto& animationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	if (bottonTargetObject.value()->renderDisabled() == true && bottonTargetObject.value()->physicsDisabled() == true) {
		bottonTargetObject.value()->enableRender();
		bottonTargetObject.value()->enablePhysics();
		animationComponent->animate(ANIMATION_ACTIVE, ANIMATE_CONTINUOUS);
	}
	else {
		bottonTargetObject.value()->disableRender();
		bottonTargetObject.value()->disablePhysics();
		animationComponent->animate(ANIMATION_IDLE, ANIMATE_CONTINUOUS);
	}

	//Since we are disabling and enabling a wall that could affect navigation
	//then refresh all navigation objects accessibility
	LevelManager::instance().refreshNavigationAccess(bottonTargetObject.value()->parentScene());

	
}
