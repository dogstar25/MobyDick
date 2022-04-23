#pragma once
#include "actions\Action.h"

#include <SDL2/SDL.h>

class ButtonInteraction : public Action
{

	void perform(GameObject*, SDL_Scancode) override;


};

