#pragma once
#include "triggers/Trigger.h"
class PlayerDeath : public Trigger
{

public:
	PlayerDeath();
	bool hasMetCriteria(Scene* scene) override;
	void execute() override;
	void reset() override;

};

