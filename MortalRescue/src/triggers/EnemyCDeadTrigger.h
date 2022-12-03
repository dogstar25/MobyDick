#pragma once
#include "triggers/Trigger.h"
class EnemyCDeadTrigger : public Trigger
{

public:
	EnemyCDeadTrigger();
	bool hasMetCriteria(Scene* scene) override;
	void execute() override;
};

