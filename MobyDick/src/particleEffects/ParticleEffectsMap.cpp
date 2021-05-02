#include "ParticleEffectsMap.h"

ParticleEffectsMap& ParticleEffectsMap::instance()
{
	static ParticleEffectsMap singletonInstance;
	return singletonInstance;
}


ParticleEffectsMap::ParticleEffectsMap()
{
	m_particleEffectMap["smoke"] = ParticleEffects::smoke;


}

void ParticleEffectsMap::addParticleEffect(std::string particleEffectKey, ParticleEffect particleEffect)
{

	m_particleEffectMap[particleEffectKey] = particleEffect;

}
