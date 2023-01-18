#include "BBTriggerFactory.h"
#include "PlayerDeath.h"

BBTriggerFactory::BBTriggerFactory()
{

}


std::shared_ptr<Trigger> BBTriggerFactory::create(std::string triggerType)
{
	std::shared_ptr<Trigger> trigger;

	if (triggerType == "PlayerDeath") {

		trigger = std::make_shared<PlayerDeath>();
	}

	else {

		trigger = TriggerFactory::create(triggerType);

	}

	return trigger;

}
