#include "HealthStationInteraction.h"
#include <iostream>
#include "game.h"

extern std::unique_ptr<Game> game;

void HealthStationInteraction::perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode)
{
	const auto& animationComponent = interactionObject->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);

	animationComponent->animate(ANIMATION_ACTIVE, ANIMATE_CONTINUOUS);


}