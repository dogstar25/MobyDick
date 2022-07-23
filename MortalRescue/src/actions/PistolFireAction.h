#pragma once
#include "actions/Action.h"
class PistolFireAction : public Action
{
public:
	PistolFireAction() = default;

	void perform(GameObject* gameObject) override;

private:

};

