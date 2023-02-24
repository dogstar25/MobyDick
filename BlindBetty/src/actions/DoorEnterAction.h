#pragma once
#pragma once
#include "actions\Action.h"

#include <SDL2/SDL.h>

class DoorEnterAction : public Action
{

	void perform(GameObject* interactingObject, GameObject* interactionObject) override;


};

