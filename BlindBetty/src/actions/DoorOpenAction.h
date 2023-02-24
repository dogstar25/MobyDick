#pragma once
#pragma once
#include "actions\Action.h"

#include <SDL2/SDL.h>

class DoorOpenAction : public Action
{

	void perform(GameObject* doorObject) override;


};

