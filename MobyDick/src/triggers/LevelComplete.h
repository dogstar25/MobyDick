#pragma once
#include "Trigger.h"
class LevelComplete :  public Trigger
{

public:
	bool hasMetCriteria() override;
	void execute() override;

};

