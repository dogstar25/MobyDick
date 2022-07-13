#include "TriggerFactory.h"



std::shared_ptr<Trigger> TriggerFactory::create(std::string triggerType)
{
	std::shared_ptr<Trigger> trigger;

	//if (triggerType == "BaseTrigger") {

	//	trigger = std::make_shared<Trigger>();
	//}
	//else {

	//	assert(false && "Trigger Type was not found");
	//}

	//Currently have no base level triggers
	assert(false && "Trigger Type was not found");

	return trigger;

}
