#pragma once
#include "Action.h"
#include <SDL2/SDL.h>

class PrimitiveMoveAction : public Action
{

public:
	PrimitiveMoveAction() = default;

	void perform(GameObject* gameObject, Json::Value actionParms) override;

};
