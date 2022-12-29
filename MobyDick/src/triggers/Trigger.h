#pragma once
#include <string>
#include <optional>
#include "../Timer.h"

class Scene;

class Trigger
{

public:
	Trigger() {};
	virtual bool hasMetCriteria(Scene* scene);
	virtual void execute() {};
	std::string getName() { return m_triggerName; }
	virtual void reset() {};

protected:
	bool m_triggerOnlyOnce{};
	bool m_hasTriggered{};
	std::string m_triggerName;
	std::optional<Timer> m_criteriaCheckTimer{};
	

};

