#pragma once
#include "Action.h"
class HeroRotateAction : public Action
{

public:
	HeroRotateAction();
	~HeroRotateAction();

	void perform(GameObject* gameObject, float angularVelocity) override;

};

