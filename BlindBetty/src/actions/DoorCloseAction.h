#pragma once
#pragma once
#include "actions\Action.h"

#include <SDL2/SDL.h>

class DoorCloseAction : public Action
{

	void perform(GameObject* interactingObject) override;


};

