#pragma once
#include "Action.h"
#include <string>

class HeroFireAction : public Action
{

public:
	HeroFireAction();
	~HeroFireAction();

	void perform(GameObject* gameObject) override;


};

