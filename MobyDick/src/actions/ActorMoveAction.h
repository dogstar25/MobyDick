#pragma once
#include "MoveAction.h"

class ActorMoveAction : public MoveAction
{

public:
	ActorMoveAction();
	ActorMoveAction(int direction, int strafe);
	~ActorMoveAction();

	void perform(GameObject* gameObject) override;

};