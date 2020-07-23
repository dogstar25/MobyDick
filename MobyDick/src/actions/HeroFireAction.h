#pragma once
#include "Action.h"

class HeroFireAction : public Action
{

public:
	HeroFireAction();
	~HeroFireAction();

	void perform(Entity entity) override;



};

