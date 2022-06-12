#include "MRTriggerFactory.h"
#include "triggers/PlayerDeath.h"

MRTriggerFactory::MRTriggerFactory()
{

}


std::shared_ptr<Trigger> MRTriggerFactory::create(std::string triggerType)
{
	std::shared_ptr<Trigger> trigger;

	if (triggerType == "SpecialTrigger") {
		trigger = std::make_shared<PlayerDeath>();
	}

	else {

		trigger = TriggerFactory::create(triggerType);

	}

	return trigger;

}
