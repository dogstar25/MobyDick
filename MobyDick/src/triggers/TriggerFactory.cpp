#include "TriggerFactory.h"
#include "LevelComplete.h"



std::shared_ptr<Trigger> TriggerFactory::create(std::string triggerType)
{
	std::shared_ptr<Trigger> trigger;

	if (triggerType == "LevelComplete") {

		trigger = std::make_shared<LevelComplete>();
	}
	else {

		assert(false && "Trigger Type was not found");
	}

	return trigger;

}
