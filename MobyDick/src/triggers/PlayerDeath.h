#pragma once
#include "Trigger.h"
class PlayerDeath : public Trigger
{

public:
	PlayerDeath();
	bool hasMetCriteria(Scene* scene) override;
	void execute() override;

};

