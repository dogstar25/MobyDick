#pragma once
#include "Trigger.h"
class LevelComplete :  public Trigger
{

public:
	LevelComplete();
	bool hasMetCriteria(Scene* scene) override;
	void execute() override;

};

