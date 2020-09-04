#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>

#include "Component.h"

class GameObject;

class VitalityComponent :  public Component
{
public:
	VitalityComponent();
	VitalityComponent(Json::Value definitionJSON);
	~VitalityComponent();

	void update() override;
	void updateFiniteLifetime();

	void setForce(float force) { m_force = force; }
	float speed() {	return m_speed; }

	std::chrono::duration<float, std::milli> lifetime() { return m_lifetime; }
	void setLifetime(std::chrono::duration<float, std::milli> lifetime) { m_lifetime = lifetime; }

	std::chrono::duration<float, std::milli> lifetimeRemaining() { return m_lifetimeRemaining;  }
	void setLifetimeRemaining(std::chrono::duration<float, std::milli> lifeRemaining) {  m_lifetimeRemaining = lifeRemaining; }

	std::chrono::steady_clock::time_point timeSnapshot() { return m_timeSnapshot; }
	void setTimeSnapshot(std::chrono::steady_clock::time_point timeSnapshot) { m_timeSnapshot = timeSnapshot; }

	bool hasInfiniteLifetime() { return m_hasInfiniteLifetime; }
	void setHasInfiniteLifetime(bool hasInfiniteLifetime) {m_hasInfiniteLifetime = hasInfiniteLifetime; }

	bool isLifetimeAlphaFade() { return m_isLifetimeAlphaFade; }
	void setIsLifetimeAlphaFade(bool isLifetimeAlphaFade) { m_isLifetimeAlphaFade = isLifetimeAlphaFade; }

private:
	float m_speed;
	float m_force;
	float m_durability;
	float m_health;
	bool m_hasInfiniteLifetime;
	bool m_isLifetimeAlphaFade;


	std::chrono::duration<float, std::milli> m_lifetime;
	std::chrono::duration<float, std::milli> m_lifetimeRemaining;
	std::chrono::steady_clock::time_point m_timeSnapshot;



};

#endif

