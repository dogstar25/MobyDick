#pragma once
#include "actions/Action.h"
class TurretFireAction : public Action
{
public:
	TurretFireAction() = default;

	void perform(GameObject* gameObject) override;

private:

};

