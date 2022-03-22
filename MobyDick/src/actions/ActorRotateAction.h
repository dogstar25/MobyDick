#pragma once
#include "Action.h"

class ActorRotateAction : public Action
{

public:
	ActorRotateAction() = default;

	void perform(GameObject* gameObject, float angularVelocity) override;


};

