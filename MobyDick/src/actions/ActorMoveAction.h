#pragma once
#include "MoveAction.h"

class ActorMoveAction : public MoveAction
{

public:
	ActorMoveAction();

	void perform(GameObject* gameObject) override;

};