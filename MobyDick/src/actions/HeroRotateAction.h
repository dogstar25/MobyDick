#pragma once
#include "Action.h"
class HeroRotateAction : public Action
{

public:
	HeroRotateAction();
	~HeroRotateAction();

	void perform(Entity entity, float angularVelocity) override;

};

