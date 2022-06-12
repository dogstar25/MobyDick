#pragma once

class Scene;

class Trigger
{

public:
	Trigger() {};
	virtual bool hasMetCriteria(Scene* scene) = 0;
	virtual void execute() = 0;

protected:
	bool m_triggerOnlyOnce{};
	bool m_hasTriggered{};

};

