#include "TriggerMap.h"
#include "PlayerDeath.h"

TriggerMap& TriggerMap::instance()
{
	static TriggerMap singletonInstance;
	return singletonInstance;
}


TriggerMap::TriggerMap()
{
	m_triggerMap["PlayerDeath"] = std::make_shared<PlayerDeath>();
}

std::shared_ptr<Trigger> TriggerMap::getTrigger(std::string triggerId)
{

	return m_triggerMap[triggerId];

}

void TriggerMap::addTrigger(std::string actionKey, std::shared_ptr<Trigger> trigger)
{

	m_triggerMap[actionKey] = trigger;

}
