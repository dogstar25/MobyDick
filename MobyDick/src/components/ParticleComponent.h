#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>
#include <memory>
#include <chrono>
#include <json/json.h>

#include "Component.h"

class GameObject;

class ParticleComponent : public Component
{
public:
	ParticleComponent(Json::Value definitionJSON);
	~ParticleComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;
	void reset();

	bool isActive() {
		return m_isActive;
	}
	void setActive(bool active) {
		m_isActive = active;
	}
	bool isAvailable() {
		return m_isAvailable;
	}
	void setAvailable(bool available) {
		m_isAvailable = available;
	}

	std::chrono::duration<float, std::milli> m_lifetime;
	std::chrono::duration<float, std::milli> m_lifetimeRemaining;
	std::chrono::steady_clock::time_point m_time_snapshot;

	bool
		m_isLifetimeAlphaFade,
		m_hasInfiniteLifetime;

private:
	bool m_isAvailable;
	bool m_isActive;
	std::string m_poolId;



};

#endif

