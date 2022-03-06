#pragma once
#include "actions/Action.h"
class TurretFireAction : public Action
{
public:
	TurretFireAction();
	~TurretFireAction();

	void perform(GameObject* gameObject) override;

private:

};

