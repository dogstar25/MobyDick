#pragma once
#include "actions/Action.h"

class ActorBoostAction : public Action
{

public:
	ActorBoostAction() = default;

	void perform(GameObject* gameObject) override;

};