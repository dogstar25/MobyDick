#pragma once
#include "Trigger.h"
class PlayerDeath : public Trigger
{

public:
	bool hasMetCriteria() override;
	void execute() override;

};

