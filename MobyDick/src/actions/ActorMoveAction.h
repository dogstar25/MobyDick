#pragma once
#include "Action.h"

class ActorMoveAction : public Action
{

public:
	ActorMoveAction() = default;

	void perform(GameObject* gameObject, int direction, int strafe) override;

};