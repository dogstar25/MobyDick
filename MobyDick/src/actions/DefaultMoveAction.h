#pragma once
#include "Action.h"
class DefaultMoveAction : public Action
{


public:
	DefaultMoveAction() = default;

	void perform(GameObject* gameObject, int direction, int strafe) override;
};

