#pragma once
#include "Action.h"

#include <box2d/box2d.h>

class DroneMoveAction : public Action
{

public:
	DroneMoveAction() = default;

	void perform(GameObject* gameObject, b2Vec2 trajectory) override;


};


