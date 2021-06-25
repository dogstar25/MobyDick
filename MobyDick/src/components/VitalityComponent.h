#ifndef VITALITY_COMPONENT_H
#define VITALITY_COMPONENT_H

#include <chrono>
#include <array>

#include "Component.h"

class GameObject;

struct LevelDefinition
{
	short levelNum;
	short resistance;
	SDL_Color color;
};

class VitalityComponent :  public Component
{
public:
	VitalityComponent();
	VitalityComponent(Json::Value definitionJSON);
	~VitalityComponent();

	void update() override;
	void inflictDamage(float damage);
	bool testResistance(float force);

	float resistance() { return m_resistance; }
	void setResistance(float resistance) { m_resistance = resistance; }
	float attackPower() {	return m_attackPower;	}
	void setAttackPower(float attackPower) { m_attackPower = attackPower; }
	float speed() {	return m_speed; }
	void setSpeed(float speed) { m_speed = speed; }
	void setHealth(float health) { m_health = health; }
	float health() { return m_health; }
	bool isBroken() { return m_isBroken; }
	void setIsBroken(bool isBroken) { m_isBroken = isBroken; }
	bool isDestroyed() { return m_isDestroyed; }
	void setIsDestroyed(bool isDestroyed) { m_isDestroyed = isDestroyed; }
	float rotateSpeed() { return m_rotationSpeed; }


	std::chrono::duration<float, std::milli> lifetime() { return m_lifetime; }
	void setLifetime(std::chrono::duration<float, std::milli> lifetime) { m_lifetime = lifetime; }

	std::chrono::duration<float, std::milli> lifetimeRemaining() { return m_lifetimeRemaining;  }
	void setLifetimeRemaining(std::chrono::duration<float, std::milli> lifeRemaining) {  m_lifetimeRemaining = lifeRemaining; }

	std::chrono::steady_clock::time_point timeSnapshot() { return m_lifeTimeTimeSnapshot; }
	void setLifeTimeSnapshot(std::chrono::steady_clock::time_point timeSnapshot) { m_lifeTimeTimeSnapshot = timeSnapshot; }

	bool hasFiniteLifetime() { return m_hasFiniteLifetime; }
	void setHasFiniteLifetime(bool hasFiniteLifetime) {m_hasFiniteLifetime = hasFiniteLifetime; }

	bool isLifetimeAlphaFade() { return m_isLifetimeAlphaFade; }
	void setIsLifetimeAlphaFade(bool isLifetimeAlphaFade) { m_isLifetimeAlphaFade = isLifetimeAlphaFade; }

private:
	float m_speed{ 0 };
	float m_rotationSpeed{};
	float m_attackPower{ 0 };
	float m_health{ 0 };
	float m_resistance{ 0 };

	//Regenerative
	bool	m_isRegenerative{ false };
	bool	m_isBroken{ false };
	bool	m_isDestroyed{false};
	float	m_regenSpeed{ 0 };
	int		m_currentLevel{ 0 };
	int		m_maxLevels{ 0 };
	std::chrono::steady_clock::time_point m_regenTimeSnapshot;
	std::array<LevelDefinition, MAX_VITALITY_LEVELS> m_regenLevels = {};

	//Temporary timed Lifetime
	bool m_hasFiniteLifetime{ false };
	bool m_isLifetimeAlphaFade{ true };
	std::chrono::duration<float, std::milli> m_lifetime;
	std::chrono::duration<float, std::milli> m_lifetimeRemaining;
	std::chrono::steady_clock::time_point m_lifeTimeTimeSnapshot;

	void _levelUp();
	void _updateFiniteLifetime();
	void _updateRegeneration();

};

#endif

