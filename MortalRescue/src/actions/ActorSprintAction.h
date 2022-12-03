#pragma once
#include "actions/Action.h"


class ActorSprintAction : public Action
{

public:
	ActorSprintAction() = default;

	void perform(GameObject* gameObject, int direction, int strafe) override;

};
