#pragma once

#include <map>
#include <memory>
#include <string>
#include <assert.h>

#include "BaseParticleEffects.h"

class ParticleEffectsMap
{
public:
	static ParticleEffectsMap& instance();
	ParticleEffectsMap();

	ParticleEffect getParticleEffect(std::string particleEffectKey) {

		return m_particleEffectMap[particleEffectKey];
	}

	void addParticleEffect(std::string particleEffectKey, ParticleEffect particleEffect);

private:

	std::map<std::string, ParticleEffect> m_particleEffectMap;

};

