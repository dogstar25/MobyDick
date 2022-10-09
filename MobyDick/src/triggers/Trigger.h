#pragma once
#include <string>

class Scene;

class Trigger
{

public:
	Trigger() {};
	virtual bool hasMetCriteria(Scene* scene) = 0;
	virtual void execute() {};
	std::string getName() { return m_triggerName; }
	virtual void reset() {};

protected:
	bool m_triggerOnlyOnce{};
	bool m_hasTriggered{};
	std::string m_triggerName;
	

};

