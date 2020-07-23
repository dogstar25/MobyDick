#pragma once
#include "Action.h"
class HeroMoveAction : public Action
{

public:
	HeroMoveAction();
	~HeroMoveAction();

	void perform(Entity entity, int direction, int strafe) override;

};

