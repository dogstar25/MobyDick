#pragma once

#include <map>
#include <memory>
#include <string>
#include <assert.h>

#include "Trigger.h"
#include "PlayerDeath.h"

class TriggerMap
{
public:
	static TriggerMap& instance();
	TriggerMap();

	std::shared_ptr<Trigger> getTrigger(std::string triggerId);
	void addTrigger(std::string triggerKey, std::shared_ptr<Trigger> trigger);

private:

	std::map<std::string, std::shared_ptr<Trigger>>m_triggerMap;

};

