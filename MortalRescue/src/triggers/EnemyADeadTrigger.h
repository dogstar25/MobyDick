#pragma once
#include "triggers/Trigger.h"
class EnemyADeadTrigger : public Trigger
{

public:
	EnemyADeadTrigger();
	bool hasMetCriteria(Scene* scene) override;

};

