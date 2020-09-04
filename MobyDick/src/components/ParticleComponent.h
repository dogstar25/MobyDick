#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>
#include <memory>
#include <chrono>
#include <vector>

#include <json/json.h>

#include "Component.h"
#include "../Globals.h"
#include "../ParticleEmission.h"
#include "../config_data/ParticleEffect.h"

class GameObject;

class ParticleComponent : public Component
{
public:
	ParticleComponent(Json::Value definitionJSON);
	~ParticleComponent();

	void update() override;
	void setType(int type) { m_type = type; }
	void setEmissionInterval(std::chrono::duration<float>);
	void addParticleEffect(ParticleEffect particleEffect);

private:
	int m_type{ParticleEmitterType::ONETIME};
	std::vector<ParticleEffect> m_particleEffects;

	std::chrono::duration<float, std::milli> m_emissionInterval{ std::chrono::duration<float>(0.F) };
	std::chrono::steady_clock::time_point m_timeSnapshot{ std::chrono::steady_clock::now() };



};






#endif

