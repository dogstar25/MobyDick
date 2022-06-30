#pragma once
#include "actions\Action.h"

#include <SDL2/SDL.h>

class HealthStationInteraction : public Action
{

	void perform(GameObject* interactingObject, GameObject* interactionObject, SDL_Scancode keyCode) override;


};