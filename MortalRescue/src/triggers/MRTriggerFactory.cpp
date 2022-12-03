#include "MRTriggerFactory.h"
#include "PlayerDeath.h"
#include "LevelComplete.h"
#include "EnemyADeadTrigger.h"
#include "EnemyCDeadTrigger.h"

MRTriggerFactory::MRTriggerFactory()
{

}


std::shared_ptr<Trigger> MRTriggerFactory::create(std::string triggerType)
{
	std::shared_ptr<Trigger> trigger;

	if (triggerType == "PlayerDeath") {

		trigger = std::make_shared<PlayerDeath>();
	}
	else if (triggerType == "LevelComplete") {

		trigger = std::make_shared<LevelComplete>();
	}
	else if (triggerType == "EnemyADeadTrigger") {

		trigger = std::make_shared<EnemyADeadTrigger>();
	}
	else if (triggerType == "EnemyCDeadTrigger") {

		trigger = std::make_shared<EnemyCDeadTrigger>();
	}

	else {

		trigger = TriggerFactory::create(triggerType);

	}

	return trigger;

}
