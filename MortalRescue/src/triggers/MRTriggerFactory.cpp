#include "MRTriggerFactory.h"
#include "PlayerDeath.h"

MRTriggerFactory::MRTriggerFactory()
{

}


std::shared_ptr<Trigger> MRTriggerFactory::create(std::string triggerType)
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
