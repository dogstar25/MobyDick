#pragma once
#include "MoveAction.h"

#include <box2d/box2d.h>

class DroneMoveAction : public MoveAction
{

public:
	DroneMoveAction();
	DroneMoveAction(b2Vec2 trajectory);
	~DroneMoveAction();

	void perform(GameObject* gameObject) override;
};

