#pragma once
#include "Action.h"
class HeroMoveAction : public Action
{

public:
	HeroMoveAction();
	~HeroMoveAction();

	void perform(GameObject* gameObject, int direction, int strafe) override;


};

