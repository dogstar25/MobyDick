#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>

#include "Component.h"

class VitalityComponent :  public Component
{
public:
	VitalityComponent();
	VitalityComponent(std::string gameObjectId);
	~VitalityComponent();

	void update() override;

	//Accessor functions
	float speed() {
		return m_speed;
	}

private:
	float
		m_speed,
		m_strength,
		m_health;

	std::chrono::duration<float, std::milli> m_lifetime;
	std::chrono::duration<float, std::milli> m_lifetimeRemaining;
	std::chrono::steady_clock::time_point time_snapshot;

	bool
		isLifetimeAlphaFade,
		hasInfiniteLifetime;


};

#endif

