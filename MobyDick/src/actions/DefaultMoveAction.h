#pragma once
#include "MoveAction.h"
class DefaultMoveAction : public MoveAction
{


public:
	DefaultMoveAction();
	DefaultMoveAction(int direction, int strafe);
	~DefaultMoveAction();

	void perform(GameObject* gameObject) override;
};

