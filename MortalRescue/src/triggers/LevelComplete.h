#pragma once
#include "triggers/Trigger.h"
class LevelComplete :  public Trigger
{

public:
	LevelComplete();
	bool hasMetCriteria(Scene* scene) override;
	void execute() override;

};

