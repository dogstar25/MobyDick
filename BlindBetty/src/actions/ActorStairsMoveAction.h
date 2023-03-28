#pragma once
#include "actions/Action.h"
#include "../GameConstants.h"

class ActorStairsMoveAction : public Action
{

public:
	ActorStairsMoveAction() = default;

	void perform(GameObject* gameObject, int direction) override;

};